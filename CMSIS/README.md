# CMSIS

Integrate CMSIS core and devices

## Folder
- `/cmsis-core`: from [CMSIS_5](https://github.com/ARM-software/CMSIS_5)
- `/cmsis-device`: from [STM32CubeF3](https://github.com/STMicroelectronics/STM32CubeF3)

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