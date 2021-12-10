#!/bin/bash

. ../../config.sh

echo "Compiling..."
riscv32-unknown-elf-gcc -nostdlib -ffreestanding -TLink.ld -o Bare Bare.s

echo "Dumping..."
riscv32-unknown-elf-objdump -D Bare > Bare.dump

echo "Generating Verilog memory image..."
riscv32-unknown-elf-objcopy -O verilog Bare Bare.vmem

echo "Cleanup and finish"
rm Bare
