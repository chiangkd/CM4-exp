CC = arm-none-eabi-gcc

MACH = cortex-m4
CFLAGS = -c -mcpu=$(MACH) -mthumb -std=gnu11 -Wall
LDFLAGS = -mcpu=$(MACH) -mthumb --specs=nano.specs -T stm32f303ze_linker.ld -lc -lnosys -Wl,-Map=final.map

all: main.o startup.o syscall.o led.o hsi.o final.elf

semi: main.o startup.o syscall.o led.o final_sh.elf

main.o: main.c
	$(CC) $(CFLAGS) -o $@ $^

led.o: led.c
	$(CC) $(CFLAGS) -o $@ $^

hsi.o: hsi.c
	$(CC) $(CFLAGS) -o $@ $^

startup.o: startup.c
	$(CC) $(CFLAGS) -o $@ $^

syscall.o: syscall.c
	$(CC) $(CFLAGS) -o $@ $^

final.elf: main.o startup.o syscall.o led.o hsi.o
	$(CC) $(LDFLAGS) -o $@ $^

final_sh.elf: main.o startup.o
	$(CC) $(LDFLAGS_SH) -o $@ $^

clean:
	rm -rf *.o *.elf *.out *.map

upload:
	openocd -f interface/stlink.cfg -f target/stm32f3x.cfg -c " program final.elf verify reset exit "

debug:
	openocd -f interface/stlink.cfg -f target/stm32f3x.cfg