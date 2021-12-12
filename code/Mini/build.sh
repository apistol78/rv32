#!/bin/bash

. ../../config.sh

echo "Compiling..."
riscv32-unknown-elf-gcc -TLink.ld -nostartfiles -lm -lc -o Mini crt0.s Mini.c 

echo "Dumping..."
riscv32-unknown-elf-objdump -D Mini > Mini.dump

echo "Generating HEX..."
riscv32-unknown-elf-objcopy -O ihex Mini Mini.hex

echo "Converting image into Verilog flat image..."
../../build/linux/ReleaseStatic/Hex2Verilog -word=32 Mini.hex Mini.vmem

echo "Cleanup and finish"
#rm Mini
