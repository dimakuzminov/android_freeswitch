LOCAL_PATH:= $(call my-dir)

libpcre_src_files := \
    libs/pcre/pcre_chartables.c \
    libs/pcre/dist/pcre_byte_order.c \
    libs/pcre/dist/pcre_compile.c \
    libs/pcre/dist/pcre_config.c \
    libs/pcre/dist/pcre_dfa_exec.c \
    libs/pcre/dist/pcre_exec.c \
    libs/pcre/dist/pcre_fullinfo.c \
    libs/pcre/dist/pcre_get.c \
    libs/pcre/dist/pcre_globals.c \
    libs/pcre/dist/pcre_jit_compile.c \
    libs/pcre/dist/pcre_maketables.c \
    libs/pcre/dist/pcre_newline.c \
    libs/pcre/dist/pcre_ord2utf8.c \
    libs/pcre/dist/pcre_refcount.c \
    libs/pcre/dist/pcre_string_utils.c \
    libs/pcre/dist/pcre_study.c \
    libs/pcre/dist/pcre_tables.c \
    libs/pcre/dist/pcre_ucd.c \
    libs/pcre/dist/pcre_valid_utf8.c \
    libs/pcre/dist/pcre_version.c \
    libs/pcre/dist/pcre_xclass.c

include $(CLEAR_VARS)
LOCAL_MODULE := libpcre
LOCAL_CFLAGS += -DHAVE_CONFIG_H
LOCAL_C_INCLUDES += \
					$(LOCAL_PATH)/libs/pcre \
					$(LOCAL_PATH)/libs/pcre/dist \

LOCAL_SRC_FILES := $(libpcre_src_files)
include $(BUILD_HOST_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := libpcre
LOCAL_CFLAGS += -DHAVE_CONFIG_H
LOCAL_C_INCLUDES += \
					$(LOCAL_PATH)/libs/pcre \
					$(LOCAL_PATH)/libs/pcre/dist \

LOCAL_SRC_FILES := $(libpcre_src_files)
include $(BUILD_STATIC_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := libpcre
LOCAL_CFLAGS += -DHAVE_CONFIG_H
LOCAL_C_INCLUDES += \
					$(LOCAL_PATH)/libs/pcre \
					$(LOCAL_PATH)/libs/pcre/dist \

LOCAL_SRC_FILES := $(libpcre_src_files)
include $(BUILD_SHARED_LIBRARY)
