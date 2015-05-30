CC=$(CROSS_COMPILE)g++
CXX=$(CROSS_COMPILE)g++
LD=$(CROSS_COMPILE)g++

LIBSTL=../gsmlib/libgsmme.a /data/android-ndk-r10d/sources/cxx-stl/gnu-libstdc++/4.6/libs/armeabi-v7a/libgnustl_static.a

BUILD ?= debug
#CXXSOURCES := $(shell ls *.cpp 2>/dev/null)
#CSOURCES := $(shell ls *.c 2>/dev/null)
#OBJECTS := $(CXXSOURCES:.cpp=.o) $(CSOURCES:.c=.o)
#DEPFILES := $(OBJECTS:%.o=.%.P)

CFLAGS_DEBUG := -g -D__DEBUGING__
CFLAGS_RELEASE := -O2
CFLAGS += -I.. -D__STDC_CONSTANT_MACROS  -DHAVE_CONFIG_H -fPIC
#CFLAGS += -std=gnu99 -D_GNU_SOURCE -D_FORTIFY_SOURCE=2  -D_POSIX_SOURCE 

debug: CFLAGS += $(CFLAGS_DEBUG)
release: CFLAGS += $(CFLAGS_RELEASE)

#CFLAGS += -DTEST_WITHOUT_GIRAFFIC
CXXFLAGS += $(CFLAGS) -fpermissive -frtti 
CXXFLAGS += -I$(NDK_ROOT)/sources/cxx-stl/gnu-libstdc++/4.6/include
CXXFLAGS += -I$(NDK_ROOT)/sources/cxx-stl/gnu-libstdc++/4.6/libs/armeabi-v7a/include
CXXFLAGS += -I$(NDK_ROOT)/sources/cxx-stl/gnu-libstdc++/4.6/include/backward
CXXFLAGS += -I$(SYSROOT)/usr/include

LDFLAGS += -L$(SYSROOT)/usr/lib -L$(NDK_ROOT)/sources/cxx-stl/gnu-libstdc++/4.6/libs/armeabi-v7a -L../gsmlib 
LDFLAGS += -Wl,-rpath-link=$(SYSROOT)/usr/lib
LDFLAGS += -Xlinker -rpath -Xlinker $(SYSROOT)/usr/lib -Wl,-rpath=/system/lib
#LDFLAGS +=  -lstdc++ -landroid -lm -ldl -llog -lc

LDFLAGS += -lstdc++ -lm -ldl -llog -lc -landroid 
#LDFLAGS += -lgsmme

.PHONY:all debug release clean


debug release:all

all: crtbegin_dynamic.o crtend_android.o gsmctl gsmpb gsmsendsms gsmsmsd gsmsmsstore

crtbegin_dynamic.o:
	ln -sf $(SYSROOT)/usr/lib/crtbegin_dynamic.o .

crtend_android.o: 
	ln -sf $(SYSROOT)/usr/lib/crtend_android.o .

gsmctl:gsmctl.o $(LIBSTL) #crtbegin_dynamic.o crtend_android.o 
	$(LINK.o) -o $@ $^

gsmpb:gsmpb.o $(LIBSTL) #crtbegin_dynamic.o crtend_android.o 
	$(LINK.o) -o $@ $^

gsmsendsms:gsmsendsms.o $(LIBSTL) #crtbegin_dynamic.o crtend_android.o 
	$(LINK.o) -o $@ $^

gsmsmsd:gsmsmsd.o $(LIBSTL) #crtbegin_dynamic.o crtend_android.o 
	$(LINK.o) -o $@ $^

gsmsmsstore:gsmsmsstore.o $(LIBSTL) #crtbegin_dynamic.o crtend_android.o 
	$(LINK.o) -o $@ $^


clean:
	rm -f gsmctl gsmpb gsmsendsms gsmsmsd gsmsmsstore *.o *~ \#*
	rm -f .*.P

$(OBJECTS):Makefile

DF=$(*F)

%.o:%.cpp
	$(COMPILE.cpp) -MMD -o $@ $<
	@cp $(DF).d .$(DF).P; sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' -e '/^$$/ d' -e 's/$$/ :/' < $(DF).d >> .$(DF).P && rm -f $(DF).d

%.o:%.c
	$(COMPILE.c) -MMD -o $@ $<
	@cp $(DF).d .$(DF).P; sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' -e '/^$$/ d' -e 's/$$/ :/' < $(DF).d >> .$(DF).P && rm -f $(DF).d

-include $(DEPFILES)
