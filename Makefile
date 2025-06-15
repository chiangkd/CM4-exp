CC = arm-none-eabi-gcc
SIZE = arm-none-eabi-size

# example code
EXAMPLE := example
INC_DIR := $(EXAMPLE)/inc
SRC_DIR := $(EXAMPLE)/src
OBJ_DIR := build

BIN := main

SRCS := main.c \
		$(wildcard src/*c) \
 		$(wildcard $(EXAMPLE)/src/*.c)

OBJS := $(patsubst %.c, $(OBJ_DIR)/%.o, $(notdir $(SRCS)))



MACH = cortex-m4
CFLAGS = -c -mcpu=$(MACH) -mthumb -std=gnu11 -Wall -Iinc -I$(EXAMPLE)/inc
LDFLAGS = -mcpu=$(MACH) -mthumb --specs=nano.specs -T stm32f303ze_linker.ld -lc -lnosys -Wl,-Map=final.map

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

$(OBJ_DIR)/%.o: $(EXAMPLE)/src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(OBJ_DIR) *.elf *.map

upload:
	openocd -f interface/stlink.cfg -f target/stm32f3x.cfg -c " program final.elf verify reset exit "

debug:
	openocd -f interface/stlink.cfg -f target/stm32f3x.cfg