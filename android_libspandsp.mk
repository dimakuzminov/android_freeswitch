LOCAL_PATH:= $(call my-dir)

# ========================================================
# spandsp shared library
# ========================================================
include $(CLEAR_VARS)
LOCAL_SRC_FILES := \
	libs/spandsp/src/plc.c \
	libs/spandsp/src/alloc.c \
	libs/spandsp/src/dtmf.c \
	libs/spandsp/src/tone_detect.c \
	libs/spandsp/src/logging.c \
	libs/spandsp/src/tone_generate.c \
	libs/spandsp/src/queue.c \
	libs/spandsp/src/dds_float.c \
	libs/spandsp/src/super_tone_rx.c \
	libs/spandsp/src/v18.c \
	libs/spandsp/src/async.c \
	libs/spandsp/src/fsk.c \
	libs/spandsp/src/dds_int.c \
	libs/spandsp/src/power_meter.c \
	libs/spandsp/src/modem_connect_tones.c \
	libs/spandsp/src/t38_core.c \
	libs/spandsp/src/t30.c \
	libs/spandsp/src/t30_logging.c \
	libs/spandsp/src/t4_rx.c \
	libs/spandsp/src/t4_tx.c \
	libs/spandsp/src/t30_api.c \
	libs/spandsp/src/t35.c \
	libs/spandsp/src/t85_encode.c \
	libs/spandsp/src/t43.c \
	libs/spandsp/src/t4_t6_decode.c \
	libs/spandsp/src/t85_decode.c \
	libs/spandsp/src/t42.c \
	libs/spandsp/src/t4_t6_encode.c \
	libs/spandsp/src/bit_operations.c \
	libs/spandsp/src/image_translate.c \
	libs/spandsp/src/timezone.c \
	libs/spandsp/src/t81_t82_arith_coding.c \
	libs/spandsp/src/t38_terminal.c \
	libs/spandsp/src/t38_gateway.c \
	libs/spandsp/src/t38_non_ecm_buffer.c \
	libs/spandsp/src/fax_modems.c \
	libs/spandsp/src/hdlc.c \
	libs/spandsp/src/silence_gen.c \
	libs/spandsp/src/v27ter_tx.c \
	libs/spandsp/src/v29tx.c \
	libs/spandsp/src/v17tx.c \
	libs/spandsp/src/crc.c \
	libs/spandsp/src/v29rx.c \
	libs/spandsp/src/v27ter_rx.c \
	libs/spandsp/src/v17rx.c \
	libs/spandsp/src/vector_float.c \
	libs/spandsp/src/complex_vector_float.c \
	libs/spandsp/src/math_fixed.c \
	libs/spandsp/src/fax.c \
	libs/spandsp/src/v8.c \
	libs/spandsp/src/t31.c \
	libs/spandsp/src/at_interpreter.c \
	libs/spandsp/src/lpc10_encode.c \
	libs/spandsp/src/lpc10_analyse.c \
	libs/spandsp/src/lpc10_placev.c \
	libs/spandsp/src/lpc10_voicing.c \
	libs/spandsp/src/lpc10_decode.c \
	libs/spandsp/src/gsm0610_encode.c \
	libs/spandsp/src/gsm0610_preprocess.c \
	libs/spandsp/src/gsm0610_lpc.c \
	libs/spandsp/src/gsm0610_short_term.c \
	libs/spandsp/src/gsm0610_long_term.c \
	libs/spandsp/src/gsm0610_rpe.c \
	libs/spandsp/src/gsm0610_decode.c \
	libs/spandsp/src/g722.c \
	libs/spandsp/src/vector_int.c \
	libs/spandsp/src/g726.c \
	libs/spandsp/src/bitstream.c \
	libs/spandsp/src/ima_adpcm.c \

LOCAL_C_INCLUDES += \
					$(LOCAL_PATH)/src/include \
					$(LOCAL_PATH)/libs/spandsp/src \
					$(LOCAL_PATH)/libs/libteletone/src \
					$(LOCAL_PATH)/libs/tiff-4.0.2/libtiff \
					$(LOCAL_PATH)/libs/apr/include \
					$(LOCAL_PATH)/libs/apr-util/include \
					$(LOCAL_PATH)/libs/sofia-sip/libsofia-sip-ua/sdp \
					$(LOCAL_PATH)/libs/sofia-sip/libsofia-sip-ua/su \
					$(LOCAL_PATH)/libs/sofia-sip/open_c \
					$(LOCAL_PATH)/libs/libtpl-1.5/src \
					$(LOCAL_PATH)/libs/apr/include/arch/unix \
					$(LOCAL_PATH)/libs/apr/include/arch/unix \
					$(LOCAL_PATH)/libs/apr/random/unix \
					$(LOCAL_PATH)/libs/apr-util/include \
					$(LOCAL_PATH)/libs/apr-util/include/private \
					$(LOCAL_PATH)/../srtp/crypto/include/ \
					$(LOCAL_PATH)/../srtp/include \
					$(LOCAL_PATH)/../curl/include \
					$(LOCAL_PATH)/../sqlite/dist \
					$(LOCAL_PATH)/../openssl/include \
					$(LOCAL_PATH)/../speex/include \
					$(LOCAL_PATH)/../regex-re2 \
					$(LOCAL_PATH)/../regex-re2/util \
					$(LOCAL_PATH)/../libctb-0.16/include \
					$(LOCAL_PATH)/../e2fsprogs/lib \
					$(LOCAL_PATH)/../jpeg \

LOCAL_MODULE_TAGS := optional
LOCAL_MODULE := libspandsp

LOCAL_SHARED_LIBRARIES := \
	libtiff \
	libjpeg \
	libz \

include $(BUILD_SHARED_LIBRARY)
