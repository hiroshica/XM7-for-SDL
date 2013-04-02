###
###	FM-7 EMULATOR "XM7"
###
###	Copyright (C) 2004 GIMONS
###	Copyright (C) 2010 K.Ohta
###	[ Makefile For SDL/Agar]

#################### Build flags ############################
# Uncomment if using OpenGL
USE_OPENGL      = Yes

# Uncomment if using OpenMP, Debian's MinGW-GCC is *not* supported,yet.
USE_OPENMP      = Yes

# Uncomment if using OpenCL for rendering
USE_OPENCL      = Yes

# Uncomment if using debugger.
WITH_DEBUGGER   = Yes

# Uncomment if BIG-ENDIAN
#BIG_ENDIAN      = Yes

# Programs
#INSTALL          = install -c 
PREFIX           = /usr/local

# Define toolchains
FMGEN_CC = gcc-4.7
FMGEN_CXX = g++-4.7
CC = /usr/local/bin/gcc
CXX = /usr/local/bin/g++

# Uncomment if cross-build
#CROSS_BUILD    = Yes
#CROSS_TARGET   = i686-w64-mingw32
#CROSS_PREFIX    = $(HOME)/src/mingw-crossbuild



# SET Arch specified
#ARCH_FLAGS       = -m32
#ARCH_FLAGS       = -mmmx -msse -msse2 -mfpmath=sse
#ARCH_FLAGS      = -march=amdfam10
# SET Arch depended flags
BUILD_SSE2       = Yes

#OS               = Windows
OS               = Linux
ARCH             = x86_64
#ARCH            = ia32

#################### UI  ############################
## Embed目的の場合はkanameやkonatuのような小さなフォントにすること
UI_FONT            = ipagp.ttf
UI_PT              = 15

STAT_FONT          = ipagp.ttf
STAT_PT            = 9

VFD_FONT           = ipagp.ttf
CMT_FONT           = ipagp.ttf


###### Include Common.
#include common.mak




