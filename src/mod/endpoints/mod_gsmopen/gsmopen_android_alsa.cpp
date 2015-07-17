#include "gsmopen.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/poll.h>
#include <sys/ioctl.h>

extern "C"{
#include <sound/asound.h>
#include "alsa_audio.h"
}

#define FREESWITCH_PERIOD_SIZE 320
#define WRITE_PERIOD_SIZE      (4*(FREESWITCH_PERIOD_SIZE)) //TODO: should be param in conf file, for msm8960 had to be larger than 1024
typedef struct __alsa_object_t {
    struct pcm      *pcm_out;
    struct pcm      *pcm_in;
    struct mixer    *mixer;
    struct pcm_ctl  *ctl;
    int             pcm_out_active;
    int             pcm_in_active;
    uint8_t         write_buffer[WRITE_PERIOD_SIZE];
    uint32_t        write_buffer_size;
} alsa_object_t;

static int debug = 0;
static int write_period = WRITE_PERIOD_SIZE;
static int read_period = FREESWITCH_PERIOD_SIZE;

static int set_params_out(private_t *tech_pvt, struct pcm *pcm) {
    struct snd_pcm_hw_params *params;
    struct snd_pcm_sw_params *sparams;
    unsigned long  periodSize;
    unsigned long  bufferSize;
    unsigned long  reqBuffSize;
    unsigned int   periodTime;
    unsigned int   bufferTime;
    params = (struct snd_pcm_hw_params*) calloc(1, sizeof(struct snd_pcm_hw_params));
    if (!params) {
        ERRORA("PCM_OUT:Failed to allocate ALSA hardware parameters!", GSMOPEN_P_LOG);
        return -ENOMEM;
    }
    param_init(params);
    param_set_mask(params, SNDRV_PCM_HW_PARAM_ACCESS, SNDRV_PCM_ACCESS_RW_INTERLEAVED);
    param_set_mask(params, SNDRV_PCM_HW_PARAM_FORMAT, pcm->format);
    param_set_mask(params, SNDRV_PCM_HW_PARAM_SUBFORMAT, SNDRV_PCM_SUBFORMAT_STD);
    if (write_period) {
        param_set_min(params, SNDRV_PCM_HW_PARAM_PERIOD_BYTES, write_period);
    } else {
        param_set_min(params, SNDRV_PCM_HW_PARAM_PERIOD_TIME, 10);
    }
    param_set_int(params, SNDRV_PCM_HW_PARAM_SAMPLE_BITS, 16);
    param_set_int(params, SNDRV_PCM_HW_PARAM_FRAME_BITS, pcm->channels * 16);
    param_set_int(params, SNDRV_PCM_HW_PARAM_CHANNELS, pcm->channels);
    param_set_int(params, SNDRV_PCM_HW_PARAM_RATE, pcm->rate);
    param_set_hw_refine(pcm, params);
    if (param_set_hw_params(pcm, params)) {
        ERRORA("PCM_OUT:cannot set hw params\n", GSMOPEN_P_LOG);
        return -errno;
    }
    if (debug) {
        param_dump(params);
    }
    pcm->buffer_size = pcm_buffer_size(params);
    pcm->period_size = pcm_period_size(params);
    pcm->period_cnt = pcm->buffer_size/pcm->period_size;
    if (debug) {
        DEBUGA_GSMOPEN("period_cnt = %d\n", GSMOPEN_P_LOG, pcm->period_cnt);
        DEBUGA_GSMOPEN("period_size = %d\n", GSMOPEN_P_LOG, pcm->period_size);
        DEBUGA_GSMOPEN("buffer_size = %d\n", GSMOPEN_P_LOG, pcm->buffer_size);
    }
    sparams = (struct snd_pcm_sw_params*) calloc(1, sizeof(struct snd_pcm_sw_params));
    if (!sparams) {
        ERRORA("PCM_OUT:Failed to allocate ALSA software parameters!\n", GSMOPEN_P_LOG);
        return -ENOMEM;
    }
    // Get the current software parameters
    sparams->tstamp_mode = SNDRV_PCM_TSTAMP_NONE;
    sparams->period_step = 1;
    sparams->avail_min = pcm->period_size/(pcm->channels * 2) ;
    sparams->start_threshold =  pcm->period_size/(pcm->channels * 2) ;
    sparams->stop_threshold =  pcm->buffer_size ;
    sparams->xfer_align =  pcm->period_size/(pcm->channels * 2) ; /* needed for old kernels */
    sparams->silence_size = 0;
    sparams->silence_threshold = 0;
    if (param_set_sw_params(pcm, sparams)) {
        ERRORA("PCM_OUT:cannot set sw params\n", GSMOPEN_P_LOG);
        return -errno;
    }
    if (debug) {
        DEBUGA_GSMOPEN("sparams->avail_min= %lu\n", GSMOPEN_P_LOG, sparams->avail_min);
        DEBUGA_GSMOPEN(" sparams->start_threshold= %lu\n", GSMOPEN_P_LOG, sparams->start_threshold);
        DEBUGA_GSMOPEN(" sparams->stop_threshold= %lu\n", GSMOPEN_P_LOG, sparams->stop_threshold);
        DEBUGA_GSMOPEN(" sparams->xfer_align= %lu\n", GSMOPEN_P_LOG, sparams->xfer_align);
        DEBUGA_GSMOPEN(" sparams->boundary= %lu\n", GSMOPEN_P_LOG, sparams->boundary);
    }
    return 0;
}

static int set_params_in(private_t *tech_pvt, struct pcm *pcm)
{
    struct snd_pcm_hw_params *params;
    struct snd_pcm_sw_params *sparams;
    unsigned long periodSize, bufferSize, reqBuffSize;
    unsigned int periodTime, bufferTime;
    params = (struct snd_pcm_hw_params*) calloc(1, sizeof(struct snd_pcm_hw_params));
    if (!params) {
        ERRORA("PCM_IN:Failed to allocate ALSA hardware parameters!\n", GSMOPEN_P_LOG);
        return -ENOMEM;
    }
    param_init(params);
    param_set_mask(params, SNDRV_PCM_HW_PARAM_ACCESS, SNDRV_PCM_ACCESS_RW_INTERLEAVED);
    param_set_mask(params, SNDRV_PCM_HW_PARAM_FORMAT, pcm->format);
    param_set_mask(params, SNDRV_PCM_HW_PARAM_SUBFORMAT, SNDRV_PCM_SUBFORMAT_STD);
    if (read_period)
        param_set_min(params, SNDRV_PCM_HW_PARAM_PERIOD_BYTES, read_period);
    else
        param_set_min(params, SNDRV_PCM_HW_PARAM_PERIOD_TIME, 10);
    param_set_int(params, SNDRV_PCM_HW_PARAM_SAMPLE_BITS, 16);
    param_set_int(params, SNDRV_PCM_HW_PARAM_FRAME_BITS, pcm->channels * 16);
    param_set_int(params, SNDRV_PCM_HW_PARAM_CHANNELS, pcm->channels);
    param_set_int(params, SNDRV_PCM_HW_PARAM_RATE, pcm->rate);
    param_set_hw_refine(pcm, params);
    if (param_set_hw_params(pcm, params)) {
        ERRORA("PCM_IN:cannot set hw params [%d:%s]\n", GSMOPEN_P_LOG, errno, strerror(errno));
        return -errno;
    }
    if (debug) {
        param_dump(params);
    }
    pcm->buffer_size = pcm_buffer_size(params);
    pcm->period_size = pcm_period_size(params);
    pcm->period_cnt = pcm->buffer_size/pcm->period_size;
    if (debug) {
        DEBUGA_GSMOPEN("period_size (%d)\n", GSMOPEN_P_LOG, pcm->period_size);
        DEBUGA_GSMOPEN(" buffer_size (%d)\n", GSMOPEN_P_LOG, pcm->buffer_size);
        DEBUGA_GSMOPEN(" period_cnt  (%d)\n", GSMOPEN_P_LOG, pcm->period_cnt);
    }
    sparams = (struct snd_pcm_sw_params*) calloc(1, sizeof(struct snd_pcm_sw_params));
    if (!sparams) {
        ERRORA("PCM_IN:Failed to allocate ALSA software parameters!\n", GSMOPEN_P_LOG);
        return -ENOMEM;
    }
    sparams->tstamp_mode = SNDRV_PCM_TSTAMP_NONE;
    sparams->period_step = 1;
    if (pcm->flags & PCM_MONO) {
        sparams->avail_min = pcm->period_size/2;
        sparams->xfer_align = pcm->period_size/2;
    } else {
        sparams->avail_min = pcm->period_size/4;
        sparams->xfer_align = pcm->period_size/4;
    }
    sparams->avail_min  = 1024;
    sparams->xfer_align = 0;
    sparams->start_threshold = 1024;
    sparams->stop_threshold = INT_MAX;
    sparams->silence_size = 0;
    sparams->silence_threshold = 0;
    if (param_set_sw_params(pcm, sparams)) {
        ERRORA("PCM_IN:cannot set sw params\n", GSMOPEN_P_LOG);
        return -errno;
    }
    if (debug) {
        DEBUGA_GSMOPEN("avail_min (%lu)\n", GSMOPEN_P_LOG, sparams->avail_min);
        DEBUGA_GSMOPEN("start_threshold (%lu)\n", GSMOPEN_P_LOG, sparams->start_threshold);
        DEBUGA_GSMOPEN("stop_threshold (%lu)\n", GSMOPEN_P_LOG, sparams->stop_threshold);
        DEBUGA_GSMOPEN("xfer_align (%lu)\n", GSMOPEN_P_LOG, sparams->xfer_align);
    }
    return 0;
}

struct mixer_ctl *alsa_audio_get_ctl(struct mixer *mixer, const char *name) {
    char *p;
    unsigned int idx = 0;
    if (isdigit(name[0])) {
        return mixer_get_nth_control(mixer, atoi(name) - 1);
    }
    p = strrchr(name, '#');
    if (p) {
        *p++ = 0;
        idx = atoi(p);
    }
    return mixer_get_control(mixer, name, idx);
}


typedef struct __mixer_command_t {
    const char* ctrl;
    const char* value;
} mixer_command_t;

#define PLAYER_HW_DEVICE "hw:0,0"

static mixer_command_t mixer_commands[] = {
    // enable GSM downlink
    {"PRI_RX_Voice Mixer CSVoice",              "1"},
    // enable GSM uplink
    {"Voice_Tx Mixer PRI_TX_Voice",             "1"},
    // enable uplink to GSM from freeswitch
    {"Incall_Music Audio Mixer MultiMedia1",    "1"},
    // enbale downlink from GSM to freeswitch
    {"MultiMedia1 Mixer VOC_REC_DL",            "1"},
    // disable device microphone
    {"DEC4 Volume",                             "0%"},
    // disable device speaker
    {"RX1 Digital Volume",                      "0%"},
    {NULL,                                      NULL}
};

int alsa_audio_mixer_set(private_t *tech_pvt) {
    struct mixer *mixer;
    struct mixer_ctl *ctl;
    const char* device = "/dev/snd/controlC0";
    mixer = mixer_open(device);
    if (!mixer) {
        ERRORA("mixer_open failed: %s\n", GSMOPEN_P_LOG, strerror(errno));
        return -1;
    }
    mixer_command_t *pos = mixer_commands;
    for (; pos->ctrl != NULL; pos++) {
        ctl = alsa_audio_get_ctl(mixer, pos->ctrl);
        if (!ctl) {
            ERRORA("get_ctl failed: %s\n", GSMOPEN_P_LOG, strerror(errno));
            mixer_close(mixer);
            return -1;
        }
        if (mixer_ctl_set_value(ctl, 1, (char**)&pos->value)) {
            ERRORA("mixer_ctl_set_value failed: %s\n", GSMOPEN_P_LOG, strerror(errno));
            return -1;
        }
    }
    mixer_close(mixer);
    return 0;
}

int alsa_audio_init(private_t *tech_pvt) {
	tech_pvt->alsa_audio_opened = 1;
    if (tech_pvt->alsa_priv == NULL) {
        int channels = 1; // MONO
        int rate = 8000; // 8 KHz 
        tech_pvt->alsa_priv = calloc(1, sizeof(alsa_object_t));
        alsa_object_t *alsa = (alsa_object_t*)tech_pvt->alsa_priv;
        const char* device = PLAYER_HW_DEVICE; //TODO: need to take from configuration
        // setup mixer for Samsung
        // TODO: need to move it for some kind of preinit script
        if (alsa_audio_mixer_set(tech_pvt)) {
            return -errno;
        }
        // open pcm_out
        alsa->pcm_out = pcm_open(PCM_OUT|PCM_MONO|PCM_NMMAP, (char*)device);
        if (!pcm_ready(alsa->pcm_out)) {
            pcm_close(alsa->pcm_out);
            alsa->pcm_out = NULL;
            return -EBADFD;
        }
        // open pcm_in
        alsa->pcm_in = pcm_open(PCM_IN|PCM_MONO, (char*)device);
        if (!pcm_ready(alsa->pcm_in)) {
            pcm_close(alsa->pcm_in);
            alsa->pcm_in = NULL;
            return -EBADFD;
        }
        // setup pcm_out device
        alsa->pcm_out->channels = 1; // MONO
        alsa->pcm_out->rate = 8000; // 8 KHz
        alsa->pcm_out->flags = PCM_OUT|PCM_MONO|PCM_NMMAP;
        alsa->pcm_out->format = SNDRV_PCM_FORMAT_S16_LE;
        if (set_params_out(tech_pvt, alsa->pcm_out)) {
            ERRORA("PCM_OUT:params setting failed\n", GSMOPEN_P_LOG);
            pcm_close(alsa->pcm_out);
            alsa->pcm_out = NULL;
            return -errno;
        }
        // setup pcm_in device
        alsa->pcm_in->channels = 1; // MONO
        alsa->pcm_in->rate = 8000; // 8 KHz
        alsa->pcm_in->flags = PCM_IN|PCM_MONO;
        alsa->pcm_in->format = SNDRV_PCM_FORMAT_S16_LE;
        if (set_params_in(tech_pvt, alsa->pcm_in)) {
            ERRORA("PCM_OUT:params setting failed\n", GSMOPEN_P_LOG);
            pcm_close(alsa->pcm_in);
            alsa->pcm_in = NULL;
            return -errno;
        }
    }
    return 0;
}

int alsa_audio_shutdown(private_t *tech_pvt) {
	tech_pvt->alsa_audio_opened = 0;
    if (tech_pvt->alsa_priv != NULL) {
        alsa_object_t *alsa = (alsa_object_t*)tech_pvt->alsa_priv;
        alsa->pcm_out_active = 0;;
        alsa->pcm_in_active = 0;;
    }
    return 0;
}

int alsa_audio_write(private_t *tech_pvt, char *data, int datalen) {
    if (tech_pvt->alsa_priv != NULL) {
        alsa_object_t *alsa = (alsa_object_t*)tech_pvt->alsa_priv;
        if (alsa->pcm_out_active == 0) {
            if (pcm_prepare(alsa->pcm_out)) {
                ERRORA("PCM_OUT:Failed in pcm_prepare\n", GSMOPEN_P_LOG);
                pcm_close(alsa->pcm_out);
                alsa->pcm_out = NULL;
                return -errno;
            }
            alsa->pcm_out_active = 1;
        }
        if (alsa->pcm_out_active == 1) {
            if (datalen != FREESWITCH_PERIOD_SIZE) {
                ERRORA("PCM_OUT: expected period size [%u], received [%u] - packet dropped\n",
                        GSMOPEN_P_LOG, FREESWITCH_PERIOD_SIZE, datalen);
                return 0;
            }
            if (alsa->write_buffer_size < WRITE_PERIOD_SIZE) {
                memcpy(alsa->write_buffer+alsa->write_buffer_size, data, datalen);
                alsa->write_buffer_size += datalen;
            }
            if (alsa->write_buffer_size == WRITE_PERIOD_SIZE) {
                if (pcm_write(alsa->pcm_out, alsa->write_buffer, WRITE_PERIOD_SIZE)) {
                    ERRORA("PCM_OUT: pcm_write failed\n", GSMOPEN_P_LOG);
                    pcm_close(alsa->pcm_out);
                    alsa->pcm_out = NULL;
                    return -errno;
                }
                alsa->write_buffer_size = 0;
            }
            return datalen;
        }
    }
    return 0;
}

int alsa_audio_read(private_t *tech_pvt, char *data, int datalen) {
    if (tech_pvt->alsa_priv != NULL) {
        alsa_object_t *alsa = (alsa_object_t*)tech_pvt->alsa_priv;
        if (alsa->pcm_in_active == 0) {
            if (pcm_prepare(alsa->pcm_in)) {
                ERRORA("PCM_IN:Failed in pcm_prepare\n", GSMOPEN_P_LOG);
                pcm_close(alsa->pcm_in);
                alsa->pcm_in = NULL;
                return -errno;
            }
            if (ioctl(alsa->pcm_in->fd, SNDRV_PCM_IOCTL_START)) {
                if (errno == EPIPE) {
                    ERRORA("PCM_IN:Failed in SNDRV_PCM_IOCTL_START\n", GSMOPEN_P_LOG);
                    /* we failed to make our window -- try to restart */
                    alsa->pcm_in->running = 0;
                } else {
                    ERRORA("PCM_IN:Error no %d \n", GSMOPEN_P_LOG, errno);
                    return -errno;
                }
            }
            alsa->pcm_in_active = 1;
        }
        if (alsa->pcm_in_active == 1) {
            if (pcm_read(alsa->pcm_in, data, datalen)) {
                ERRORA("PCM_IN: pcm_write failed\n", GSMOPEN_P_LOG);
                pcm_close(alsa->pcm_in);
                alsa->pcm_in = NULL;
                return -errno;
            }
            return datalen;
        }
    }
    return 0;
}
