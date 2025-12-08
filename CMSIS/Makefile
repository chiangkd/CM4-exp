CC = arm-none-eabi-gcc
AS = arm-none-eabi-gcc
SIZE = arm-none-eabi-size

CMSIS_DIRS := cmsis-core cmsis-device
SRC_DIR := src

STARTUP := cmsis-device/startup_stm32f303xe.s
SYSTEM  := cmsis-device/system_stm32f3xx.c

STM_TYPE := STM32F303xE


OBJ_DIR := build

C_SRCS := main.c \
		$(wildcard $(SRC_DIR)/*c) \
		$(SYSTEM)

# startup
SRCS := $(C_SRCS) $(STARTUP)

OBJS := $(patsubst %.c,$(OBJ_DIR)/%.o,$(notdir $(C_SRCS))) \
        $(OBJ_DIR)/startup_stm32f303xe.o


MACH = cortex-m4
CFLAGS = -c -g -mcpu=$(MACH) -mthumb -std=gnu11 -Wall -Iinc $(foreach d,$(CMSIS_DIRS),-I$(d)) -D$(STM_TYPE)



ASFLAGS = -mcpu=$(MACH) -mthumb

LDFLAGS = -mcpu=$(MACH) -mthumb --specs=nano.specs  \
		  -T stm32f303ze_linker.ld -lc -lnosys 		\
		  -Wl,-Map=final.map

BIN := final.elf

all: $(OBJ_DIR) $(BIN)

# Link object files
$(BIN): $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o $@
	$(SIZE) $@

# Compile .c to .o
$(OBJ_DIR)/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: cmsis-device/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/startup_stm32f303xe.o: $(STARTUP)
	$(AS) $(ASFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(OBJ_DIR) *.elf *.map

upload:
	openocd -f interface/stlink.cfg -f target/stm32f3x.cfg -c " program final.elf verify reset exit "

debug:
	openocd -f interface/stlink.cfg -f target/stm32f3x.cfg
