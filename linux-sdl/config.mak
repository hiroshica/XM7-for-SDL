###
###	FM-7 EMULATOR "XM7"
###
###	Copyright (C) 2004 GIMONS
###	Copyright (C) 2010 K.Ohta
###	[ Makefile GTK For SDL]
#XM7_VER		= 3
#OPTION		= -DMOUSE -DMR2 -DFDDSND -DMEMWATCH
OPTION		= -DMOUSE -DMR2 -DFDDSND
OPTION          += -DUSE_OPENGL
#OPTION		= -DMOUSE -DMR2
OPTION		+= -DUSE_AGAR
# IF SET Little endian
OPTION		+= -D_M_IX86

INSTALL          = install -c 
PREFIX           = /usr/local
SHAREDIR         = $(PREFIX)/share/xm7/

## SVNの時はコメントアウト外す
SVNVER = `svnversion`


## Embed目的の場合はkanameやkonatuのような小さなフォントにすること
UI_FONT            = ipagp.ttf
UI_PT              = 13

STAT_FONT          = ipagp.ttf
STAT_PT            = 18

VFD_FONT           = ipagui.ttf
CMT_FONT           = ipagui.ttf
FONTPATH           = /usr/local/share/xm7:/usr/share/fonts/X11/misc/:/usr/share/fonts/X11/75dpi/:/usr/share/fonts/X11/Type1/:/usr/share/fonts/truetype/ipafont:/usr/share/fonts/truetype/mona:/usr/share/fonts/truetype/misaki/:.:./.xm7/:./xm7/:


LDFLAGS = 

#CC = gcc
#CXX = g++
CC = gcc-4.7
CXX = g++-4.7
AR = ar

CFLAGS = -DXM7_VER=$(XM7_VER) $(OPTION) -D_XWIN -DNDEBUG -DUIDIR=\"$(SHAREDIR)\" -DRSSDIR=\"$(SHAREDIR)\"
CFLAGS += -DUI_FONT=\"$(UI_FONT)\" -DFUNC_FONT=\"$(FUNC_FONT)\" -DSTAT_FONT=\"$(STAT_FONT)\" -DVFD_FONT=\"$(VFD_FONT)\" -DCMT_FONT=\"$(CMT_FONT)\"
CFLAGS += -DFONTPATH=\"$(FONTPATH)\"
CFLAGS += -DUI_PT=$(UI_PT) -DSTAT_PT=$(STAT_PT)

ifdef SVNVER
CFLAGS += -DSVNVER=\"$(SVNVER)\"
endif

CFLAGS +=  -fpermissive -I. 
CFLAGS += -I../ui-agar/ -I../sdl/ -I../vm/ -I../xm7-debugger/ -I../fmgen/ -I../
CFLAGS += `sdl-config --cflags`
#CFLAGS += -I/usr/local/include/SDL
CFLAGS += `agar-config --cflags`
# OpenMP
CFLAGS += -fopenmp

CFLAGS_DEBUG = -pg -g -O0 
CFLAGS_DEBUG += $(CFLAGS)

CXXFLAGS_RELEASE =  -O3
CXXFLAGS_RELEASE += -floop-block -fprefetch-loop-arrays -fbranch-probabilities
#CFLAGS_RELEASE += -pthread

CFLAGS_RELEASE =  -O3
CFLAGS_RELEASE +=  -floop-block -fprefetch-loop-arrays -fbranch-probabilities
#CFLAGS_RELEASE += -floop-block -fprefetch-loop-arrays
#CFLAGS_RELEASE += -pthread

# Architecture Depend Flag
CFLAGS_RELEASE += -march=amdfam10
CFLAGS_RELEASE += -minline-all-stringops

CXXFLAGS_RELEASE += -march=amdfam10
CXXFLAGS_RELEASE += -minline-all-stringops 



CFLAGS_RELEASE += $(CFLAGS)
CXXFLAGS_RELEASE += $(CFLAGS)

ASFLAGS =	-DXM7_VER=$(XM7_VER) -f elf -d _XWIN
