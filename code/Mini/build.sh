#!/bin/bash

. ../../config.sh

echo "Compiling..."
riscv32-unknown-elf-gcc \
    -Wl,-static -fdata-sections \
    -ffunction-sections \
    -Wl,--gc-sections \
    -Os \
    -Tlink_ram.ld \
    -nostartfiles \
    -lm \
    -lc \
    -o Mini \
    crt0.s \
    crt0.c \
    system.c \
    Mini.c 

echo "Dumping..."
riscv32-unknown-elf-objdump -D Mini > Mini.dump

echo "Generating HEX..."
riscv32-unknown-elf-objcopy -O ihex Mini Mini.hex

echo "Cleanup and finish"
rm Mini
