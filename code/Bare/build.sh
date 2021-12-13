#!/bin/bash

. ../../config.sh

echo "Compiling..."
riscv32-unknown-elf-gcc -nostdlib -ffreestanding -TLink.ld -o Bare Bare.s Bare.c

echo "Dumping..."
riscv32-unknown-elf-objdump -D Bare > Bare.dump

echo "Generating HEX image..."
riscv32-unknown-elf-objcopy -O ihex Bare Bare.hex

echo "Converting image into Verilog flat image..."
../../build/linux/ReleaseStatic/Hex2Verilog -word=32 Bare.hex Bare.vmem

echo "Cleanup and finish"
rm Bare
