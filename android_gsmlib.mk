LOCAL_PATH:= $(call my-dir)


# ========================================================
# copy gnustl shared libary to our system
# ========================================================
$(shell cp \
	$(LOCAL_PATH)/../../prebuilts/ndk/8/sources/cxx-stl/gnu-libstdc++/4.7/libs/armeabi-v7a/libgnustl_shared.so \
	$(TARGET_OUT_SHARED_LIBRARIES)/ \
)

# ========================================================
# libgsmme.so shared library
# ========================================================
include $(CLEAR_VARS)
LOCAL_SRC_FILES := \
	libs/android_gsmlib/gsmlib/gsm_at.cpp \
	libs/android_gsmlib/gsmlib/gsm_cb.cpp \
	libs/android_gsmlib/gsmlib/gsm_error.cpp \
	libs/android_gsmlib/gsmlib/gsm_event.cpp \
	libs/android_gsmlib/gsmlib/gsm_me_ta.cpp \
	libs/android_gsmlib/gsmlib/gsm_nls.cpp \
	libs/android_gsmlib/gsmlib/gsm_parser.cpp \
	libs/android_gsmlib/gsmlib/gsm_phonebook.cpp \
	libs/android_gsmlib/gsmlib/gsm_sms_codec.cpp \
	libs/android_gsmlib/gsmlib/gsm_sms.cpp \
	libs/android_gsmlib/gsmlib/gsm_sms_store.cpp \
	libs/android_gsmlib/gsmlib/gsm_sorted_phonebook_base.cpp \
	libs/android_gsmlib/gsmlib/gsm_sorted_phonebook.cpp \
	libs/android_gsmlib/gsmlib/gsm_sorted_sms_store.cpp \
	libs/android_gsmlib/gsmlib/gsm_unix_serial.cpp \
	libs/android_gsmlib/gsmlib/gsm_util.cpp \
	libs/android_gsmlib/gsmlib/gsm_win32_serial.cpp

LOCAL_C_INCLUDES += \
	$(LOCAL_PATH)/../../prebuilts/ndk/8/sources/cxx-stl/gnu-libstdc++/4.7/include \
	$(LOCAL_PATH)/../../prebuilts/ndk/8/sources/cxx-stl/gnu-libstdc++/4.7/libs/armeabi-v7a/include \
	$(LOCAL_PATH)/../../prebuilts/ndk/8/sources/cxx-stl/gnu-libstdc++/include/backward \
	$(LOCAL_PATH)/libs/android_gsmlib/gsmlib \
	$(LOCAL_PATH)/libs/android_gsmlib \

LOCAL_CFLAGS += -frtti -fexceptions -DHAVE_CONFIG_H
LOCAL_LDFLAGS += -L$(TARGET_OUT_SHARED_LIBRARIES) -lgnustl_shared
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE := libgsmme
include $(BUILD_SHARED_LIBRARY)
# ========================================================
# gsmctl executable
# ========================================================
include $(CLEAR_VARS)
LOCAL_SRC_FILES := libs/android_gsmlib/apps/gsmctl.cpp
LOCAL_C_INCLUDES += \
	$(LOCAL_PATH)/../../prebuilts/ndk/8/sources/cxx-stl/gnu-libstdc++/4.7/include \
	$(LOCAL_PATH)/../../prebuilts/ndk/8/sources/cxx-stl/gnu-libstdc++/4.7/libs/armeabi-v7a/include \
	$(LOCAL_PATH)/../../prebuilts/ndk/8/sources/cxx-stl/gnu-libstdc++/include/backward \
	$(LOCAL_PATH)/libs/android_gsmlib/gsmlib \
	$(LOCAL_PATH)/libs/android_gsmlib \

LOCAL_SHARED_LIBRARIES := libgsmme
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE := gsmctl
LOCAL_CFLAGS += -frtti -fexceptions -DHAVE_CONFIG_H
LOCAL_LDFLAGS += -L$(TARGET_OUT_SHARED_LIBRARIES) -lgnustl_shared
include $(BUILD_EXECUTABLE)
# ========================================================
# gsmsmsd executable
# ========================================================
include $(CLEAR_VARS)
LOCAL_SRC_FILES := libs/android_gsmlib/apps/gsmsmsd.cpp
LOCAL_C_INCLUDES += \
	$(LOCAL_PATH)/../../prebuilts/ndk/8/sources/cxx-stl/gnu-libstdc++/4.7/include \
	$(LOCAL_PATH)/../../prebuilts/ndk/8/sources/cxx-stl/gnu-libstdc++/4.7/libs/armeabi-v7a/include \
	$(LOCAL_PATH)/../../prebuilts/ndk/8/sources/cxx-stl/gnu-libstdc++/include/backward \
	$(LOCAL_PATH)/libs/android_gsmlib/gsmlib \
	$(LOCAL_PATH)/libs/android_gsmlib \

LOCAL_MODULE_TAGS := optional
LOCAL_MODULE := gsmsmsd
LOCAL_SHARED_LIBRARIES := libgsmme
LOCAL_CFLAGS += -frtti -fexceptions -DHAVE_CONFIG_H
LOCAL_LDFLAGS += -L$(TARGET_OUT_SHARED_LIBRARIES) -lgnustl_shared
include $(BUILD_EXECUTABLE)
# ========================================================
# gsmpb executable
# ========================================================
include $(CLEAR_VARS)
LOCAL_SRC_FILES := libs/android_gsmlib/apps/gsmpb.cpp
LOCAL_C_INCLUDES += \
	$(LOCAL_PATH)/../../prebuilts/ndk/8/sources/cxx-stl/gnu-libstdc++/4.7/include \
	$(LOCAL_PATH)/../../prebuilts/ndk/8/sources/cxx-stl/gnu-libstdc++/4.7/libs/armeabi-v7a/include \
	$(LOCAL_PATH)/../../prebuilts/ndk/8/sources/cxx-stl/gnu-libstdc++/include/backward \
	$(LOCAL_PATH)/libs/android_gsmlib/gsmlib \
	$(LOCAL_PATH)/libs/android_gsmlib \

LOCAL_MODULE_TAGS := optional
LOCAL_MODULE := gsmpb
LOCAL_SHARED_LIBRARIES := libgsmme
LOCAL_CFLAGS += -frtti -fexceptions -DHAVE_CONFIG_H
LOCAL_LDFLAGS += -L$(TARGET_OUT_SHARED_LIBRARIES) -lgnustl_shared
include $(BUILD_EXECUTABLE)
# ========================================================
# gsmsmsstore executable
# ========================================================
include $(CLEAR_VARS)
LOCAL_SRC_FILES := libs/android_gsmlib/apps/gsmsmsstore.cpp
LOCAL_C_INCLUDES += \
	$(LOCAL_PATH)/../../prebuilts/ndk/8/sources/cxx-stl/gnu-libstdc++/4.7/include \
	$(LOCAL_PATH)/../../prebuilts/ndk/8/sources/cxx-stl/gnu-libstdc++/4.7/libs/armeabi-v7a/include \
	$(LOCAL_PATH)/../../prebuilts/ndk/8/sources/cxx-stl/gnu-libstdc++/include/backward \
	$(LOCAL_PATH)/libs/android_gsmlib/gsmlib \
	$(LOCAL_PATH)/libs/android_gsmlib \

LOCAL_MODULE_TAGS := optional
LOCAL_MODULE := gsmsmsstore
LOCAL_SHARED_LIBRARIES := libgsmme
LOCAL_CFLAGS += -frtti -fexceptions -DHAVE_CONFIG_H
LOCAL_LDFLAGS += -L$(TARGET_OUT_SHARED_LIBRARIES) -lgnustl_shared
include $(BUILD_EXECUTABLE)
# ========================================================
# gsmsendsms executable
# ========================================================
include $(CLEAR_VARS)
LOCAL_SRC_FILES := libs/android_gsmlib/apps/gsmsendsms.cpp
LOCAL_C_INCLUDES += \
	$(LOCAL_PATH)/../../prebuilts/ndk/8/sources/cxx-stl/gnu-libstdc++/4.7/include \
	$(LOCAL_PATH)/../../prebuilts/ndk/8/sources/cxx-stl/gnu-libstdc++/4.7/libs/armeabi-v7a/include \
	$(LOCAL_PATH)/../../prebuilts/ndk/8/sources/cxx-stl/gnu-libstdc++/include/backward \
	$(LOCAL_PATH)/libs/android_gsmlib/gsmlib \
	$(LOCAL_PATH)/libs/android_gsmlib \

LOCAL_MODULE_TAGS := optional
LOCAL_MODULE := gsmsendsms
LOCAL_SHARED_LIBRARIES := libgsmme
LOCAL_CFLAGS += -frtti -fexceptions -DHAVE_CONFIG_H
LOCAL_LDFLAGS += -L$(TARGET_OUT_SHARED_LIBRARIES) -lgnustl_shared
include $(BUILD_EXECUTABLE)
