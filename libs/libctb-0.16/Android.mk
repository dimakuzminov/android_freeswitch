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
	src/fifo.cpp \
	src/getopt.cpp \
	src/iobase.cpp \
	src/kbhit.cpp \
	src/portscan.cpp \
	src/serportx.cpp \
	src/linux/serport.cpp \
	src/linux/timer.cpp \

LOCAL_C_INCLUDES += \
	$(LOCAL_PATH)/../../prebuilts/ndk/8/sources/cxx-stl/gnu-libstdc++/4.7/include \
	$(LOCAL_PATH)/../../prebuilts/ndk/8/sources/cxx-stl/gnu-libstdc++/4.7/libs/armeabi-v7a/include \
	$(LOCAL_PATH)/../../prebuilts/ndk/current/platforms/android-8/arch-arm/usr/include \
	$(LOCAL_PATH)/../../prebuilts/ndk/8/sources/cxx-stl/gnu-libstdc++/include/backward \
	$(LOCAL_PATH)/include/ \
	$(KERNEL_OUT)/usr/include \

LOCAL_CFLAGS += -frtti -fexceptions
LOCAL_LDFLAGS += -L$(TARGET_OUT_SHARED_LIBRARIES) -lgnustl_shared
LOCAL_SHARED_LIBRARIES := libssh
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE := libctb
include $(BUILD_SHARED_LIBRARY)
