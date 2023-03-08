# Makefile generated by the project manager
#


## core information

PRJ_FILE       = final.wpj
ifeq ($(wildcard Makefile),)
PRJ_DIR        = ..
else
PRJ_DIR        = .
endif
PRJ_TYPE       = vxApp
PRJ_OBJS       = my_tcp.o my_timer.o mem.o main.o my_task.o
BUILD_SPEC     = SIMNTgnu
TGT_DIR        = $(WIND_BASE)/target




## build-configuration info

ifeq ($(BUILD_SPEC),SIMNTgnu)
CPU              = SIMNT
TOOL             = gnu
TOOL_FAMILY      = gnu
DEFAULT_RULE     = final.out
endif

include $(TGT_DIR)/h/make/defs.project

## build-configuration info

ifeq ($(BUILD_SPEC),SIMNTgnu)
AR               = arsimpc
AS               = ccsimpc
CC               = ccsimpc
CC_ARCH_SPEC     = -mpentium
CFLAGS           = -g -mpentium -ansi -fno-builtin -fno-defer-pop -I. -I$(WIND_BASE)/target/h/ -DCPU=SIMNT -DTOOL_FAMILY=gnu -DTOOL=gnu
CFLAGS_AS        = -g -mpentium -ansi -fno-builtin -fno-defer-pop -P -xassembler-with-cpp -I. -I$(WIND_BASE)/target/h/ -DCPU=SIMNT -DTOOL_FAMILY=gnu -DTOOL=gnu
CPP              = ccsimpc -E -P
LD               = ldsimpc
LDFLAGS          = --subsystem=windows
LD_PARTIAL       = ccsimpc -r -nostdlib
LD_PARTIAL_FLAGS = -r
NM               = nmsimpc -g
OPTION_DEFINE_MACRO = -D
OPTION_DEPEND    = -M -w
OPTION_GENERATE_DEPENDENCY_FILE = -MD
OPTION_INCLUDE_DIR = -I
OPTION_LANG_C    = -xc
OPTION_UNDEFINE_MACRO = -U
SIZE             = sizesimpc
TOOL_FAMILY      = gnu
POST_BUILD_RULE  = 
PRJ_LIBS         = 
endif

# override make definitions only below this line


# override make definitions only above this line
include $(TGT_DIR)/h/make/rules.project



## build-configuration info

ifeq ($(BUILD_SPEC),SIMNTgnu)
my_tcp.o:
	$(CC) -g -mpentium -ansi -fno-builtin -fno-defer-pop -I. -I$(WIND_BASE)/target/h/ -DCPU=SIMNT -DTOOL_FAMILY=gnu -DTOOL=gnu -c $(PRJ_DIR)/my_tcp.c
my_timer.o:
	$(CC) -g -mpentium -ansi -fno-builtin -fno-defer-pop -I. -I$(WIND_BASE)/target/h/ -DCPU=SIMNT -DTOOL_FAMILY=gnu -DTOOL=gnu -c $(PRJ_DIR)/my_timer.c
mem.o:
	$(CC) -g -mpentium -ansi -fno-builtin -fno-defer-pop -I. -I$(WIND_BASE)/target/h/ -DCPU=SIMNT -DTOOL_FAMILY=gnu -DTOOL=gnu -c $(PRJ_DIR)/mem.c
main.o:
	$(CC) -g -mpentium -ansi -fno-builtin -fno-defer-pop -I. -I$(WIND_BASE)/target/h/ -DCPU=SIMNT -DTOOL_FAMILY=gnu -DTOOL=gnu -c $(PRJ_DIR)/main.c
my_task.o:
	$(CC) -g -mpentium -ansi -fno-builtin -fno-defer-pop -I. -I$(WIND_BASE)/target/h/ -DCPU=SIMNT -DTOOL_FAMILY=gnu -DTOOL=gnu -c $(PRJ_DIR)/my_task.c
endif


## dependencies

my_tcp.o: $(PRJ_DIR)/my_tcp.c \
	$(WIND_BASE)/host/$(WIND_HOST_TYPE)/lib/gcc-lib/i386-pc-mingw32/gcc-2.96/include/stddef.h \
	$(WIND_BASE)/host/$(WIND_HOST_TYPE)/lib/gcc-lib/i386-pc-mingw32/gcc-2.96/include/stdarg.h \
	$(PRJ_DIR)/my_tcp.h

my_timer.o: $(PRJ_DIR)/my_timer.c \
	$(WIND_BASE)/host/$(WIND_HOST_TYPE)/lib/gcc-lib/i386-pc-mingw32/gcc-2.96/include/stddef.h \
	$(WIND_BASE)/host/$(WIND_HOST_TYPE)/lib/gcc-lib/i386-pc-mingw32/gcc-2.96/include/stdarg.h \
	$(PRJ_DIR)/my_timer.h

mem.o: $(PRJ_DIR)/mem.c \
	$(WIND_BASE)/host/$(WIND_HOST_TYPE)/lib/gcc-lib/i386-pc-mingw32/gcc-2.96/include/stddef.h \
	$(WIND_BASE)/host/$(WIND_HOST_TYPE)/lib/gcc-lib/i386-pc-mingw32/gcc-2.96/include/stdarg.h \
	$(PRJ_DIR)/mem.h

main.o: $(PRJ_DIR)/main.c \
	$(WIND_BASE)/host/$(WIND_HOST_TYPE)/lib/gcc-lib/i386-pc-mingw32/gcc-2.96/include/stddef.h \
	$(WIND_BASE)/host/$(WIND_HOST_TYPE)/lib/gcc-lib/i386-pc-mingw32/gcc-2.96/include/stdarg.h \
	$(PRJ_DIR)/my_tcp.h \
	$(PRJ_DIR)/mem.h \
	$(PRJ_DIR)/my_timer.h

my_task.o: $(PRJ_DIR)/my_task.c


## user defined rules
