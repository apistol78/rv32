#!/bin/bash

. ../../config.sh

echo "Compiling..."
riscv32-unknown-elf-gcc -ffreestanding -TLink.ld -lm -lc -o Mini Mini.c

echo "Dumping..."
riscv32-unknown-elf-objdump -D Mini > Mini.dump

echo "Generating HEX..."
riscv32-unknown-elf-objcopy -O ihex Mini Mini.hex

echo "Cleanup and finish"
rm Mini
