LOCAL_PATH:= $(call my-dir)

# ========================================================
# mod_sofia loadable shared library
# ========================================================
include $(CLEAR_VARS)
LOCAL_SRC_FILES := \
	src/mod/endpoints/mod_sofia/mod_sofia.c \
	src/mod/endpoints/mod_sofia/sofia_glue.c \
	src/mod/endpoints/mod_sofia/sofia_presence.c \
	src/mod/endpoints/mod_sofia/sofia.c \
	src/mod/endpoints/mod_sofia/rtp.c \
	src/mod/endpoints/mod_sofia/sip-dig.c \
	src/mod/endpoints/mod_sofia/sofia_reg.c \
	src/mod/endpoints/mod_sofia/sofia_media.c \

LOCAL_C_INCLUDES += \
					$(LOCAL_PATH)/src/include \
					$(LOCAL_PATH)/libs/spandsp/src \
					$(LOCAL_PATH)/libs/libteletone/src \
					$(LOCAL_PATH)/libs/tiff-4.0.2/libtiff \
					$(LOCAL_PATH)/libs/apr/include \
					$(LOCAL_PATH)/libs/apr-util/include \
					$(LOCAL_PATH)/libs/sofia-sip/libsofia-sip-ua/url \
					$(LOCAL_PATH)/libs/sofia-sip/libsofia-sip-ua/sdp \
					$(LOCAL_PATH)/libs/sofia-sip/libsofia-sip-ua/su \
					$(LOCAL_PATH)/libs/sofia-sip/libsofia-sip-ua/nua \
					$(LOCAL_PATH)/libs/sofia-sip/libsofia-sip-ua/sip \
					$(LOCAL_PATH)/libs/sofia-sip/libsofia-sip-ua/msg \
					$(LOCAL_PATH)/libs/sofia-sip/libsofia-sip-ua/nta \
					$(LOCAL_PATH)/libs/sofia-sip/libsofia-sip-ua/nea \
					$(LOCAL_PATH)/libs/sofia-sip/libsofia-sip-ua/soa \
					$(LOCAL_PATH)/libs/sofia-sip/libsofia-sip-ua/iptsec \
					$(LOCAL_PATH)/libs/sofia-sip/libsofia-sip-ua/tport \
					$(LOCAL_PATH)/libs/sofia-sip/libsofia-sip-ua/bnf \
					$(LOCAL_PATH)/libs/sofia-sip/libsofia-sip-ua/ipt \
					$(LOCAL_PATH)/libs/sofia-sip/libsofia-sip-ua/sresolv \
					$(LOCAL_PATH)/libs/sofia-sip/open_c \
					$(LOCAL_PATH)/libs/libtpl-1.5/src \
					$(LOCAL_PATH)/libs/apr/include/arch/unix \
					$(LOCAL_PATH)/libs/apr/include/arch/unix \
					$(LOCAL_PATH)/libs/apr/random/unix \
					$(LOCAL_PATH)/libs/apr-util/include \
					$(LOCAL_PATH)/libs/apr-util/include/private \
					$(LOCAL_PATH)/libs/libnatpmp/ \
					$(LOCAL_PATH)/../android_gsmlib/ \
					$(LOCAL_PATH)/../srtp/crypto/include \
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
					$(LOCAL_PATH)/include/ \

LOCAL_MODULE_TAGS := optional
LOCAL_MODULE := mod_sofia

LOCAL_SHARED_LIBRARIES := \
	libsofia-sip-ua \
	libfreeswitch \

include $(BUILD_SHARED_LIBRARY)
