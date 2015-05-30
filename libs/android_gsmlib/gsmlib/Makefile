CC=$(CROSS_COMPILE)g++
CXX=$(CROSS_COMPILE)g++
LD=$(CROSS_COMPILE)g++
AR=$(CROSS_COMPILE)ar

BUILD ?= debug
CXXSOURCES := $(shell ls *.cpp 2>/dev/null)
CSOURCES := $(shell ls *.c 2>/dev/null)
OBJECTS := $(CXXSOURCES:.cpp=.o) $(CSOURCES:.c=.o)
DEPFILES := $(OBJECTS:%.o=.%.P)
TARGET ?= libgsmme.so
TARGET2 ?= libgsmme.a

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

LDFLAGS += -L$(SYSROOT)/usr/lib -L$(NDK_ROOT)/sources/cxx-stl/gnu-libstdc++/4.6/libs/armeabi-v7a
LDFLAGS += -Wl,-rpath-link=$(SYSROOT)/usr/lib
LDFLAGS += -Xlinker -rpath -Xlinker $(SYSROOT)/usr/lib 
LDFLAGS += -lgnustl_static -lstdc++ -landroid -lm -ldl -llog -lc -shared
.PHONY:all debug release clean


debug release:all

all:$(TARGET) $(TARGET2)

$(TARGET):$(OBJECTS)
	ln -sf $(SYSROOT)/usr/lib/crtbegin_so.o .
	ln -sf $(SYSROOT)/usr/lib/crtend_so.o .
	$(LINK.o) -o $@ $^

$(TARGET2):$(OBJECTS)
	$(AR) qsc $@ $^

clean:
	rm -f $(TARGET) *.o *~ \#*
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
