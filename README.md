# CM4-exp

CM4-exp is an experimental project to learn MCU control on Cortex M4 device ([STM32F303ZE](https://www.st.com/en/microcontrollers-microprocessors/stm32f303ze.html)). Debug with [OpenOCD](https://openocd.org/) and [Saleae Logic 16](https://www.saleae.com/?srsltid=AfmBOorA8-j5SQwJtDKi4IJjFSRWM6KEGix1lJ8B-GzaEoA3FFcqMixS) (Logic Analyzer, LA)

## Build
```shell
make all    # Build binary file
make clean  # Clean object
make upload # Upload binary file to device
make debug  # Execute openocd
```

## Debug with openocd

**Terminal 1**
```shell
make debug
```

**Terminal 2**
```shell
arm-none-eabi-gdb final.elf
(gdb) target remote localhost:3333
(gdb) load
(gdb) monitor reset halt
```

## Measure HSI clock

![image](https://github.com/chiangkd/CM4-exp/blob/main/img/hsi.png)
- We can observe HSI clock's frequency is 8MHz (sample rate: 16MHz)

## Reference
- [OpenOCD General Command Line](https://openocd.org/doc/html/General-Commands.html)
- [Cortex -M4 Devices Generic User Guide Generic User Guide](https://developer.arm.com/documentation/dui0553/latest/)
- [Udemy - Mastering Microcontroller and Embedded Driver Development](https://www.udemy.com/course/mastering-microcontroller-with-peripheral-driver-development/learn/lecture/22179246?start=0#overview)
- [Udemy - Embedded Systems Programming on ARM Cortex-M3/M4 Processor](https://www.udemy.com/course/embedded-system-programming-on-arm-cortex-m3m4/learn/lecture/19391802?start=0#overview)
