LOCAL_PATH:= $(call my-dir)

# ========================================================
# tiff shared library
# ========================================================
include $(CLEAR_VARS)
LOCAL_SRC_FILES := \
	libs/tiff-4.0.2/libtiff/tif_write.c \
	libs/tiff-4.0.2/libtiff/tif_read.c \
	libs/tiff-4.0.2/libtiff/tif_error.c \
	libs/tiff-4.0.2/libtiff/tif_unix.c \
	libs/tiff-4.0.2/libtiff/tif_tile.c \
	libs/tiff-4.0.2/libtiff/tif_strip.c \
	libs/tiff-4.0.2/libtiff/tif_dirread.c \
	libs/tiff-4.0.2/libtiff/tif_swab.c \
	libs/tiff-4.0.2/libtiff/tif_flush.c \
	libs/tiff-4.0.2/libtiff/tif_open.c \
	libs/tiff-4.0.2/libtiff/tif_aux.c \
	libs/tiff-4.0.2/libtiff/tif_warning.c \
	libs/tiff-4.0.2/libtiff/tif_dirinfo.c \
	libs/tiff-4.0.2/libtiff/tif_dir.c \
	libs/tiff-4.0.2/libtiff/tif_dirwrite.c \
	libs/tiff-4.0.2/libtiff/tif_compress.c \
	libs/tiff-4.0.2/libtiff/tif_close.c \
	libs/tiff-4.0.2/libtiff/tif_codec.c \
	libs/tiff-4.0.2/libtiff/tif_dumpmode.c \
	libs/tiff-4.0.2/libtiff/tif_lzw.c \
	libs/tiff-4.0.2/libtiff/tif_packbits.c \
	libs/tiff-4.0.2/libtiff/tif_thunder.c \
	libs/tiff-4.0.2/libtiff/tif_next.c \
	libs/tiff-4.0.2/libtiff/tif_jpeg.c \
	libs/tiff-4.0.2/libtiff/tif_ojpeg.c \
	libs/tiff-4.0.2/libtiff/tif_fax3.c \
	libs/tiff-4.0.2/libtiff/tif_zip.c \
	libs/tiff-4.0.2/libtiff/tif_pixarlog.c \
	libs/tiff-4.0.2/libtiff/tif_luv.c \
	libs/tiff-4.0.2/libtiff/tif_predict.c \
	libs/tiff-4.0.2/libtiff/tif_fax3sm.c \

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
					$(LOCAL_PATH)/../jpeg \
					$(LOCAL_PATH)/../zlib \

LOCAL_MODULE_TAGS := optional
LOCAL_MODULE := libtiff

LOCAL_SHARED_LIBRARIES := \
	libjpeg \
	libz \

include $(BUILD_SHARED_LIBRARY)
