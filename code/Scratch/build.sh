#!/bin/bash

. ../../config.sh

echo "Compiling..."
riscv32-unknown-elf-gcc -Os -nostdlib -fdata-sections -ffunction-sections -Wl,--gc-sections -TLink.ld -o Scratch Scratch.c

echo "Dumping..."
riscv32-unknown-elf-objdump -D Scratch > Scratch.dump
