# Application name
APPL ?= DSpotter
BOARD = iotdk
#TOOLCHAIN?=gnu

#
# root dir of embARC
#
ifndef HOME
HOME= $(USERPROFILE)
endif

EMBARC_ROOT = ../

MID_SEL = common
STACKSZ = 16384
HEAPSZ = 34208

OLEVEL ?= Os
APPL_DEFINES += -DENABLE_BANNER=0

# application source dirs
APPL_CSRC_DIR = .
APPL_ASMSRC_DIR = .

# application include dirs
APPL_INC_DIR = .

APPL_LIBS= $(TOOLCHAIN)/libDSpotter.a
# include current project makefile
COMMON_COMPILE_PREREQUISITES += makefile

### Options above must be added before include options.mk ###
# include key embARC build system makefile
include $(EMBARC_ROOT)/options/options.mk