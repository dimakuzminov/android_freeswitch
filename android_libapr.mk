LOCAL_PATH:= $(call my-dir)

# ========================================================
# apr shared library
# ========================================================
include $(CLEAR_VARS)
LOCAL_SRC_FILES := \
	libs/apr/threadproc/unix/proc.c \
	libs/apr/threadproc/unix/procsup.c \
	libs/apr/threadproc/unix/signals.c \
	libs/apr/threadproc/unix/thread.c \
	libs/apr/threadproc/unix/threadpriv.c \
	libs/apr/memory/unix/apr_pools.c \
	libs/apr/file_io/unix/copy.c \
	libs/apr/file_io/unix/dir.c \
	libs/apr/file_io/unix/fileacc.c \
	libs/apr/file_io/unix/filedup.c \
	libs/apr/file_io/unix/filepath.c \
	libs/apr/file_io/unix/filepath_util.c \
	libs/apr/file_io/unix/filestat.c \
	libs/apr/file_io/unix/flock.c \
	libs/apr/file_io/unix/fullrw.c \
	libs/apr/file_io/unix/mktemp.c \
	libs/apr/file_io/unix/open.c \
	libs/apr/file_io/unix/pipe.c \
	libs/apr/file_io/unix/readwrite.c \
	libs/apr/file_io/unix/seek.c \
	libs/apr/file_io/unix/tempdir.c \
	libs/apr/strings/apr_cpystrn.c \
	libs/apr/strings/apr_fnmatch.c \
	libs/apr/strings/apr_snprintf.c \
	libs/apr/strings/apr_strings.c \
	libs/apr/strings/apr_strnatcmp.c \
	libs/apr/strings/apr_strtok.c \
	libs/apr/random/unix/apr_random.c \
	libs/apr/random/unix/sha2.c \
	libs/apr/random/unix/sha2_glue.c \
	libs/apr/user/unix/groupinfo.c \
	libs/apr/user/unix/userinfo.c \
	libs/apr/locks/unix/global_mutex.c \
	libs/apr/locks/unix/proc_mutex.c \
	libs/apr/locks/unix/thread_cond.c \
	libs/apr/locks/unix/thread_mutex.c \
	libs/apr/locks/unix/thread_rwlock.c \
	libs/apr/time/unix/time.c \
	libs/apr/time/unix/timestr.c \
	libs/apr/atomic/unix/apr_atomic.c \
	libs/apr/tables/apr_hash.c \
	libs/apr/tables/apr_tables.c \
	libs/apr/support/unix/waitio.c \
	libs/apr/misc/unix/charset.c \
	libs/apr/misc/unix/env.c \
	libs/apr/misc/unix/errorcodes.c \
	libs/apr/misc/unix/getopt.c \
	libs/apr/misc/unix/otherchild.c \
	libs/apr/misc/unix/rand.c \
	libs/apr/misc/unix/start.c \
	libs/apr/misc/unix/version.c \
	libs/apr/network_io/unix/inet_ntop.c \
	libs/apr/network_io/unix/inet_pton.c \
	libs/apr/network_io/unix/multicast.c \
	libs/apr/network_io/unix/sendrecv.c \
	libs/apr/network_io/unix/sockaddr.c \
	libs/apr/network_io/unix/sockets.c \
	libs/apr/network_io/unix/sockopt.c \
	libs/apr/poll/unix/epoll.c \
	libs/apr/poll/unix/kqueue.c \
	libs/apr/poll/unix/poll.c \
	libs/apr/poll/unix/port.c \
	libs/apr/poll/unix/select.c \

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
					$(LOCAL_PATH)/../e2fsprogs/lib \

LOCAL_MODULE_TAGS := optional
LOCAL_MODULE := libapr

include $(BUILD_SHARED_LIBRARY)
