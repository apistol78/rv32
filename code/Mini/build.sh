#!/bin/sh

. ../../config.sh

riscv32-unknown-elf-gcc -ffreestanding -TLink.ld -lm -lc -o Mini Mini.c #Startup.s Mini.c
riscv32-unknown-elf-objdump -D Mini > Mini.dump
riscv32-unknown-elf-objcopy -O ihex Mini Mini.hex
rm Mini
