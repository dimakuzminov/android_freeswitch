LOCAL_PATH:= $(call my-dir)

# ========================================================
# switch shared library
# ========================================================
include $(CLEAR_VARS)
LOCAL_SRC_FILES := \
	src/switch_apr.c \
	src/switch_time.c \
	src/switch_core.c \
	src/switch_core_memory.c \
	src/switch_utils.c \
	src/switch_log.c \
	src/switch_core_hash.c \
	src/switch_event.c \
	src/switch_profile.c \
	src/switch_core_session.c \
	src/switch_xml.c \
	src/switch_loadable_module.c \
	src/switch_core_rwlock.c \
	src/switch_channel.c \
	src/switch_core_io.c \
	src/switch_console.c \
	src/switch_mprintf.c \
	src/switch_version.c \
	src/switch_core_cert.c \
	src/switch_core_sqldb.c \
	src/switch_scheduler.c \
	src/switch_rtp.c \
	src/switch_nat.c \
	src/switch_core_media.c \
	src/switch_odbc.c \
	src/switch_pgsql.c \
	src/switch_curl.c \
	src/switch_core_state_machine.c \
	src/switch_regex.c \
	src/switch_core_media_bug.c \
	src/switch_buffer.c \
	src/switch_hashtable.c \
	src/switch_json.c \
	src/switch_ivr_async.c \
	src/switch_core_codec.c \
	src/switch_ivr.c \
	src/switch_resample.c \
	src/switch_ivr_play_say.c \
	src/switch_caller.c \
	src/switch_dso.c \
	src/switch_core_db.c \
	src/switch_stun.c \
	src/switch_stfu.c \
	src/switch_core_port_allocator.c \
	src/switch_core_timer.c \
	src/switch_ivr_bridge.c \
	src/switch_core_asr.c \
	src/switch_core_event_hook.c \
	src/switch_core_file.c \
	src/switch_ivr_say.c \
	src/switch_core_speech.c \
	src/switch_ivr_originate.c \
	src/switch_limit.c \
	src/switch_xml_config.c \
	src/switch_ivr_menu.c \

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
					$(LOCAL_PATH)/../pcre \
					$(LOCAL_PATH)/../srtp/include \
					$(LOCAL_PATH)/../curl/include \
					$(LOCAL_PATH)/../sqlite/dist \
					$(LOCAL_PATH)/../openssl/include \
					$(LOCAL_PATH)/../speex/include \
					$(LOCAL_PATH)/../regex-re2 \
					$(LOCAL_PATH)/../libctb-0.16/include \
					$(LOCAL_PATH)/..//e2fsprogs/lib \

LOCAL_MODULE_TAGS := optional
LOCAL_MODULE := libfreeswitch

LOCAL_SHARED_LIBRARIES := \
	libgsmme \
	libapr \
	libapr-utils \
	libpcre \
	libext2_uuid \
	libsqlite \
	libssl \
	libssh \
	libcrypto \
	libesl \
	libtpl \
	libspandsp \
	libctb \
	libnatpmp \
	libminiupnpc \
	libsofia-sip-ua \
	libteletone \
	libspeexresampler \
	libdl \
	libfreeswitch \

LOCAL_STATIC_LIBRARIES := \
	libcurl \
	libspeex \

include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_SRC_FILES := \
	src/switch.c \

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
					$(LOCAL_PATH)/..//e2fsprogs/lib \

LOCAL_MODULE_TAGS := optional
LOCAL_MODULE := freeswitch

LOCAL_SHARED_LIBRARIES := \
	libgsmme \
	libapr \
	libapr-utils \
	libpcre \
	libext2_uuid \
	libsqlite \
	libssl \
	libssh \
	libcrypto \
	libesl \
	libtpl \
	libspandsp \
	libctb \
	libnatpmp \
	libminiupnpc \
	libsofia-sip-ua \
	libteletone \
	libspeexresampler \
	libdl \
	libfreeswitch \

include $(BUILD_EXECUTABLE)
