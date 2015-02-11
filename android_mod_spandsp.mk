LOCAL_PATH:= $(call my-dir)

# ========================================================
# mod_spandsp loadable shared library
# ========================================================
include $(CLEAR_VARS)
LOCAL_SRC_FILES := \
	src/mod/applications/mod_spandsp/mod_spandsp.c \
	src/mod/applications/mod_spandsp/mod_spandsp_dsp.c \
	src/mod/applications/mod_spandsp/mod_spandsp_fax.c \
	src/mod/applications/mod_spandsp/mod_spandsp_modem.c \
	src/mod/applications/mod_spandsp/mod_spandsp_codecs.c \
	src/mod/applications/mod_spandsp/udptl.c \

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
					$(LOCAL_PATH)/libs/libnatpmp/ \
					$(LOCAL_PATH)/../android_gsmlib/ \
					$(LOCAL_PATH)/../srtp/crypto/include \
					$(LOCAL_PATH)/../srtp/include \
					$(LOCAL_PATH)/../curl/include \
					$(LOCAL_PATH)/../sqlite/dist \
					$(LOCAL_PATH)/../openssl/include \
					$(LOCAL_PATH)/../speex/include \
					$(LOCAL_PATH)/../libctb-0.16/include \
					$(LOCAL_PATH)/../e2fsprogs/lib \
					$(LOCAL_PATH)/../jpeg \
					$(LOCAL_PATH)/include/ \

LOCAL_MODULE_TAGS := optional
LOCAL_MODULE := mod_spandsp

LOCAL_SHARED_LIBRARIES := \
	libspandsp \
	libfreeswitch \

include $(BUILD_SHARED_LIBRARY)
