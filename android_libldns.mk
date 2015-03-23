LOCAL_PATH:= $(call my-dir)

# ========================================================
# sofia-sip-ua shared library
# ========================================================
include $(CLEAR_VARS)
LOCAL_SRC_FILES := \
	libs/libldns/dname.c \
	libs/libldns/rdata.c \
	libs/libldns/host2str.c \
	libs/libldns/str2host.c \
	libs/libldns/parse.c \
	libs/libldns/util.c \
	libs/libldns/buffer.c \
	libs/libldns/dnssec.c \
	libs/libldns/rr.c \
	libs/libldns/rr_functions.c \
	libs/libldns/packet.c \
	libs/libldns/host2wire.c \
	libs/libldns/wire2host.c \
	libs/libldns/dnssec_verify.c \
	libs/libldns/resolver.c \
	libs/libldns/net.c \
	libs/libldns/tsig.c \
	libs/libldns/keys.c \
	libs/libldns/rbtree.c \
	libs/libldns/dnssec_zone.c \
	libs/libldns/zone.c \
	libs/libldns/error.c \
	libs/libldns/sha1.c \
	libs/libldns/sha2.c \

LOCAL_C_INCLUDES += \
					$(LOCAL_PATH)/../openssl/include \
					$(LOCAL_PATH)/libs/libldns \

LOCAL_MODULE_TAGS := optional
LOCAL_MODULE := libldns

LOCAL_SHARED_LIBRARIES := \
	libssl \
	libssh \
	libcrypto \

include $(BUILD_SHARED_LIBRARY)
