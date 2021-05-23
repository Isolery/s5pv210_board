.PHONY: all clean

BUILD = led

CROSS_COMPILE 	?= arm-none-linux-gnueabi-

MKDIR =  mkdir
RM = rm
RMFLAGS = -rf

CC 				:= $(CROSS_COMPILE)gcc
LD				:= $(CROSS_COMPILE)ld
OBJCOPY 		:= $(CROSS_COMPILE)objcopy
OBJDUMP 		:= $(CROSS_COMPILE)objdump

INCDIRS 		:= s5pv210 \
			       bsp/led \
				   bsp/clock \
				   bsp/sdram
				   			   
SRCDIRS			:= project \
				   bsp/led \
				   bsp/clock \
				   bsp/sdram

DIR_OBJS = objs
DIR_BUILDS = builds
DIR_DEPS = deps
DIRS = $(DIR_OBJS) $(DIR_BUILDS) $(DIR_DEPS)

INCLUDE			:= $(patsubst %, -I %, $(INCDIRS))

BUILD := $(addprefix $(DIR_BUILDS)/, $(BUILD))

SFILES			:= $(foreach dir, $(SRCDIRS), $(wildcard $(dir)/*.S))
CFILES			:= $(foreach dir, $(SRCDIRS), $(wildcard $(dir)/*.c))

SFILENDIR		:= $(notdir  $(SFILES))
CFILENDIR		:= $(notdir  $(CFILES))

SOBJS = $(SFILENDIR:.S=.o)
COBJS = $(CFILENDIR:.c=.o)
OBJS = $(SOBJS) $(COBJS)
OBJS := $(addprefix $(DIR_OBJS)/, $(OBJS))

DEPS = $(CFILENDIR:.c=.dep)
DEPS := $(addprefix $(DIR_DEPS)/, $(DEPS))

VPATH			:= $(SRCDIRS)

CFLAGS          := -Wall
CPPFLAGS        := -Wa, -Wall -nostdlib -fno-builtin

LDFLAGS         := -L /home/book/s5pv210_sdk/ToolChain/arm-2009q3/lib/gcc/arm-none-linux-gnueabi/4.4.1 -lgcc

ifeq ("$(wildcard $(DIR_OBJS))", "")
DEP_DIR_OBJS := $(DIR_OBJS)
endif

ifeq ("$(wildcard $(DIR_BUILDS))", "")
DEP_DIR_BUILDS := $(DIR_BUILDS)
endif

ifeq ("$(wildcard $(DIR_DEPS))", "")
DEP_DIR_DEPS := $(DIR_DEPS)
endif

all: $(BUILD)

ifneq ($(MAKECMDGOALS), clean)
include $(DEPS)
endif

$(DIRS):
	$(MKDIR) $@

$(BUILD): $(DEP_DIR_BUILDS) $(OBJS)
	$(LD) -T s5pv210.lds -o $(BUILD).elf $(filter %.o, $^) $(LDFLAGS)
	$(OBJCOPY) -O binary -S $(BUILD).elf $(BUILD).bin
	$(OBJDUMP) -D -m arm $(BUILD).elf > $(BUILD).dis

$(DIR_OBJS)/%.o: $(DEP_DIR_OBJS) %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -march=armv4 -c -O2 $(INCLUDE) -o $@ $(filter %.c, $^)

$(DIR_OBJS)/%.o: $(DEP_DIR_OBJS) %.S
	$(CC) $(CPPFLAGS) $(CFLAGS) -march=armv4 -c -O2 -o $@ $(filter %.S, $^)


$(DIR_DEPS)/%.dep: $(DEP_DIR_DEPS) %.c
	@echo "Creating $@ ..."
	@set -e; \
	$(RM) $(RMFLAGS) $@.tmp ; \
	gcc $(INCLUDE) -E -MM $(filter %.c, $^) > $@.tmp ; \
	sed 's,\(.*\)\.o[ :]*,objs/\1.o $@: ,g' < $@.tmp > $@ ; \
	$(RM) $(RMFLAGS) $@.tmp

clean:
	$(RM) $(RMFLAGS) $(DIRS) $(BUILD)
