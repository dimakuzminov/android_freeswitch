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
include $(LOCAL_PATH)/android_libtpl.mk
include $(LOCAL_PATH)/android_libtiff.mk
include $(LOCAL_PATH)/android_libspandsp.mk
include $(LOCAL_PATH)/android_libnatpmp.mk
include $(LOCAL_PATH)/android_libminiupnpc.mk
include $(LOCAL_PATH)/android_libsofia-sip-ua.mk
include $(LOCAL_PATH)/android_libteletone.mk
include $(LOCAL_PATH)/android_libiconv.mk
# include freeswitch executable
include $(LOCAL_PATH)/android_freeswitch.mk
# include freeswitch mods
include $(LOCAL_PATH)/android_mod_gsmopen.mk
include $(LOCAL_PATH)/android_mod_console.mk
include $(LOCAL_PATH)/android_mod_logfile.mk
include $(LOCAL_PATH)/android_mod_enum.mk
include $(LOCAL_PATH)/android_mod_cdr_csv.mk
include $(LOCAL_PATH)/android_mod_event_socket.mk
include $(LOCAL_PATH)/android_mod_sofia.mk
include $(LOCAL_PATH)/android_mod_loopback.mk
include $(LOCAL_PATH)/android_mod_commands.mk
include $(LOCAL_PATH)/android_mod_conference.mk
include $(LOCAL_PATH)/android_mod_db.mk
include $(LOCAL_PATH)/android_mod_dptools.mk
include $(LOCAL_PATH)/android_mod_expr.mk
include $(LOCAL_PATH)/android_mod_fifo.mk
include $(LOCAL_PATH)/android_mod_hash.mk
include $(LOCAL_PATH)/android_mod_voicemail.mk
include $(LOCAL_PATH)/android_mod_esf.mk
include $(LOCAL_PATH)/android_mod_fsv.mk
include $(LOCAL_PATH)/android_mod_valet_parking.mk
include $(LOCAL_PATH)/android_mod_httapi.mk
include $(LOCAL_PATH)/android_mod_dialplan_xml.mk
include $(LOCAL_PATH)/android_mod_dialplan_asterisk.mk
include $(LOCAL_PATH)/android_mod_spandsp.mk
