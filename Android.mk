LOCAL_PATH:= $(call my-dir)

# ========================================================
# copy gnustl shared libary to our system
# ========================================================
$(shell cp \
	$(LOCAL_PATH)/../../prebuilts/ndk/8/sources/cxx-stl/gnu-libstdc++/4.7/libs/armeabi-v7a/libgnustl_shared.so \
	$(TARGET_OUT_SHARED_LIBRARIES)/ \
)

# include libs
include $(LOCAL_PATH)/android_libapr.mk
include $(LOCAL_PATH)/android_libapr-utils.mk
include $(LOCAL_PATH)/android_libesl.mk
#include $(LOCAL_PATH)/android_libtpl.mk
#include $(LOCAL_PATH)/android_libspandsp.mk
#include $(LOCAL_PATH)/android_libnatpmp.mk
#include $(LOCAL_PATH)/android_libminiupnpc.mk
#include $(LOCAL_PATH)/android_libsofia-sip-ua.mk
#include $(LOCAL_PATH)/android_libteletone.mk
#include $(LOCAL_PATH)/android_libiconv.mk
# include freeswitch executable
#include $(LOCAL_PATH)/android_freeswitch.mk
# include freeswitch mods
#include $(LOCAL_PATH)/android_mod_gsmopen.mk
