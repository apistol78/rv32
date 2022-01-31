#!/bin/bash

. ../../config.sh

echo "Compiling..."
riscv32-unknown-elf-gcc -Os -fdata-sections -ffunction-sections -Wl,--gc-sections -T../Runtime/Link.ld -o Five timer.c main.c

echo "Dumping..."
riscv32-unknown-elf-objdump -D Five > Five.dump


