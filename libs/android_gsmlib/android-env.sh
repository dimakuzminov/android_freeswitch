#!/bin/bash
#export ANDROID_ROOT=/data/cyanogen
export NDK_ROOT=/data/android-ndk-r10d

export SYSROOT=$NDK_ROOT/platforms/android-19/arch-arm
export TOOLCHAIN=$NDK_ROOT/toolchains/arm-linux-androideabi-4.6/prebuilt/linux-x86_64

export PATH=$PATH:$NDK_ROOT:$SYSROOT/bin:$TOOLCHAIN/bin
export CROSS_COMPILE=arm-linux-androideabi-
export NDK_TOOLCHAIN_VERSION=8
export ANDROID_ABI=armeabi-v7a
export CT_CANADIAN=y
export CT_TOOLCHAIN_TYPE="canadian"
#./configure --host=arm-linux-androideabi \
#    CFLAGS="-nostdlib" \

#    CC=${CROSS_COMPILE}gcc \
#    LD=${CROSS_COMPILE}ld \


#    CPPFLAGS="-I/data/cyanogen/prebuilts/ndk/8/platforms/android-17/arch-arm/usr/include/" \
#    LDFLAGS="-Wl,-rpath-link=/data/cyanogen/prebuilts/ndk/8/platforms/android-17/arch-arm/usr/lib/ -L/data/cyanogen/prebuilts/ndk/8/platforms/android-17/arch-arm/usr/lib" \
#    LIBS="-lc"
