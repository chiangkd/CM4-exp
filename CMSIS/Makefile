CC = arm-none-eabi-gcc
AS = arm-none-eabi-gcc
SIZE = arm-none-eabi-size

CMSIS_DIRS := cmsis-core cmsis-device freertos

SRC_DIR := src

STARTUP := cmsis-device/startup_stm32f303xe.s
SYSTEM  := cmsis-device/system_stm32f3xx.c

FREERTOS_SRCS := 					\
    freertos/list.c 			\
    freertos/queue.c 			\
    freertos/tasks.c 			\
    freertos/timers.c 		\
    freertos/event_groups.c 	\
	freertos/heap_4.c 		\
    freertos/portable/port.c

STM_TYPE := STM32F303xE


OBJ_DIR := build

C_SRCS := main.c \
		$(wildcard $(SRC_DIR)/*c) \
		$(SYSTEM)	\
		$(FREERTOS_SRCS)

# startup
SRCS := $(C_SRCS) $(STARTUP)

OBJS := $(patsubst %.c,$(OBJ_DIR)/%.o,$(notdir $(C_SRCS))) \
        $(OBJ_DIR)/startup_stm32f303xe.o \

# OBJS = build/main.o build/buzzer.o build/nyan_cat.o \
#        build/spi.o build/ssd1306.o build/systick.o \
#        build/event_groups.o build/list.o build/queue.o \
#        build/tasks.o build/timers.o \
#        build/system_stm32f3xx.o build/startup_stm32f303xe.o


MACH = cortex-m4
FPU_FLAGS = -mfpu=fpv4-sp-d16 -mfloat-abi=hard
CFLAGS = -c -g -mcpu=$(MACH) -mthumb $(FPU_FLAGS) -std=gnu11 -Wall -Iinc $(foreach d,$(CMSIS_DIRS),-I$(d)) -D$(STM_TYPE)
CFLAGS += -Ifreertos/inc

ASFLAGS = -mcpu=$(MACH) -mthumb $(FPU_FLAGS) 

LDFLAGS = -mcpu=$(MACH) -mthumb $(FPU_FLAGS) --specs=nano.specs  \
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

$(OBJ_DIR)/%.o: freertos/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: freertos/portable/%.c
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
