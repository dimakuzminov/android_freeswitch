LOCAL_PATH:= $(call my-dir)


# ========================================================
# copy gnustl shared libary to our system
# ========================================================
$(shell cp \
	$(LOCAL_PATH)/../../prebuilts/ndk/8/sources/cxx-stl/gnu-libstdc++/4.7/libs/armeabi-v7a/libgnustl_shared.so \
	$(TARGET_OUT_SHARED_LIBRARIES)/ \
)

# ========================================================
# libctb.so shared library
# ========================================================
include $(CLEAR_VARS)
LOCAL_SRC_FILES := \
	libs/libctb-0.16/src/fifo.cpp \
	libs/libctb-0.16/src/getopt.cpp \
	libs/libctb-0.16/src/iobase.cpp \
	libs/libctb-0.16/src/kbhit.cpp \
	libs/libctb-0.16/src/portscan.cpp \
	libs/libctb-0.16/src/serportx.cpp \
	libs/libctb-0.16/src/linux/serport.cpp \
	libs/libctb-0.16/src/linux/timer.cpp \

LOCAL_C_INCLUDES += \
	$(LOCAL_PATH)/../../prebuilts/ndk/8/sources/cxx-stl/gnu-libstdc++/4.7/include \
	$(LOCAL_PATH)/../../prebuilts/ndk/8/sources/cxx-stl/gnu-libstdc++/4.7/libs/armeabi-v7a/include \
	$(LOCAL_PATH)/../../prebuilts/ndk/current/platforms/android-8/arch-arm/usr/include \
	$(LOCAL_PATH)/../../prebuilts/ndk/8/sources/cxx-stl/gnu-libstdc++/include/backward \
	$(LOCAL_PATH)/libs/libctb-0.16/include/ \
	$(KERNEL_OUT)/usr/include \

LOCAL_CFLAGS += -frtti -fexceptions
LOCAL_LDFLAGS += -L$(TARGET_OUT_SHARED_LIBRARIES) -lgnustl_shared
LOCAL_SHARED_LIBRARIES := libssh
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE := libctb
include $(BUILD_SHARED_LIBRARY)
