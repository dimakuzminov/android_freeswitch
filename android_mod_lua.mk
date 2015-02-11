LOCAL_PATH:= $(call my-dir)

# ========================================================
# mod_lua loadable shared library
# ========================================================

include $(CLEAR_VARS)
LOCAL_SRC_FILES := \
	src/mod/languages/mod_lua/freeswitch_lua.cpp \
	src/mod/languages/mod_lua/mod_lua_wrap.cpp \
	src/mod/languages/mod_lua/mod_lua.cpp \
	src/mod/languages/mod_lua/lua/lapi.c \
	src/mod/languages/mod_lua/lua/lstate.c \
	src/mod/languages/mod_lua/lua/lauxlib.c \
	src/mod/languages/mod_lua/lua/linit.c \
	src/mod/languages/mod_lua/lua/lobject.c \
	src/mod/languages/mod_lua/lua/ldo.c \
	src/mod/languages/mod_lua/lua/lgc.c \
	src/mod/languages/mod_lua/lua/ltm.c \
	src/mod/languages/mod_lua/lua/lvm.c \
	src/mod/languages/mod_lua/lua/ltable.c \
	src/mod/languages/mod_lua/lua/lstring.c \
	src/mod/languages/mod_lua/lua/lfunc.c \
	src/mod/languages/mod_lua/lua/lzio.c \
	src/mod/languages/mod_lua/lua/ldump.c \
	src/mod/languages/mod_lua/lua/ldebug.c \
	src/mod/languages/mod_lua/lua/lmem.c \
	src/mod/languages/mod_lua/lua/llex.c \
	src/mod/languages/mod_lua/lua/lbaselib.c \
	src/mod/languages/mod_lua/lua/loadlib.c \
	src/mod/languages/mod_lua/lua/lcorolib.c \
	src/mod/languages/mod_lua/lua/ltablib.c \
	src/mod/languages/mod_lua/lua/liolib.c \
	src/mod/languages/mod_lua/lua/loslib.c \
	src/mod/languages/mod_lua/lua/lstrlib.c \
	src/mod/languages/mod_lua/lua/lbitlib.c \
	src/mod/languages/mod_lua/lua/lmathlib.c \
	src/mod/languages/mod_lua/lua/ldblib.c \
	src/mod/languages/mod_lua/lua/lctype.c \
	src/mod/languages/mod_lua/lua/lundump.c \
	src/mod/languages/mod_lua/lua/lparser.c \
	src/mod/languages/mod_lua/lua/lopcodes.c \
	src/mod/languages/mod_lua/lua/lcode.c \

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
					$(LOCAL_PATH)/src/mod/languages/mod_lua/lua \
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

LOCAL_CFLAGS += -DLUA_USE_LINUX
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE := mod_lua

LOCAL_SHARED_LIBRARIES := \
	libfreeswitch \
	libdl \

include $(BUILD_SHARED_LIBRARY)
