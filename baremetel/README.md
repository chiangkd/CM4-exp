# Baremetel

Hand-writen device peripheral layer and driver.

## Header file
In this project, a minimum device peripheral access layer is described in `/driver/inc`. Browse CMSIS can get full information
- [cmsis-device-f3/Include/stm32f303xe.h](https://github.com/STMicroelectronics/cmsis-device-f3/blob/5558e64e3675a1e1fcb1c71f468c7c407c1b1134/Include/stm32f303xe.h)

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

![image](https://github.com/chiangkd/CM4-exp/blob/main/baremetel/img/hsi.png)
- We can observe HSI clock's frequency is 8MHz (sample rate: 16MHz)
