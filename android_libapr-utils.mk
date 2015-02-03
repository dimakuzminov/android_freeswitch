LOCAL_PATH:= $(call my-dir)

# ========================================================
# switch_apr shared library
# ========================================================
include $(CLEAR_VARS)
LOCAL_SRC_FILES := \
	libs/apr-util/crypto/apr_md4.c \
	libs/apr-util/crypto/apr_md5.c \
	libs/apr-util/crypto/apr_sha1.c \
	libs/apr-util/crypto/getuuid.c \
	libs/apr-util/crypto/uuid.c \
	libs/apr-util/xlate/xlate.c \
	libs/apr-util/misc/apu_version.c \
	libs/apr-util/misc/apr_rmm.c \
	libs/apr-util/misc/apr_reslist.c \
	libs/apr-util/misc/apr_queue.c \
	libs/apr-util/misc/apr_date.c \
	libs/apr-util/encoding/apr_base64.c \
	libs/apr/misc/unix/rand.c

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
LOCAL_MODULE := libapr-utils
LOCAL_SHARED_LIBRARIES := libapr libssh libext2_uuid libssl libcrypto

include $(BUILD_SHARED_LIBRARY)
