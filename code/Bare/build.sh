#!/bin/bash

. ../../config.sh

echo "Compiling..."
riscv32-unknown-elf-gcc -Os -TLink.ld -o Bare \
    CRC.c   \
    GPIO.c  \
    I2C.c   \
    Main.c  \
    Print.c \
    SD.c    \
    UART.c  \
    Video.c

echo "Dumping..."
riscv32-unknown-elf-objdump -D Bare > Bare.dump

echo "Generating HEX image..."
riscv32-unknown-elf-objcopy -O ihex Bare Bare.hex

echo "Cleanup and finish"
rm Bare
