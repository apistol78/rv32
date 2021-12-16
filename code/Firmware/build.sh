#!/bin/bash

. ../../config.sh

echo "Compiling..."
riscv32-unknown-elf-gcc -Os -nostdlib -ffreestanding -TLink.ld -o Firmware Startup.s Firmware.c

echo "Dumping..."
riscv32-unknown-elf-objdump -D Firmware > Firmware.dump

echo "Generating HEX image..."
riscv32-unknown-elf-objcopy -O ihex Firmware Firmware.hex

echo "Converting image into Verilog flat image..."
../../build/linux/ReleaseStatic/Hex2Verilog -word=32 Firmware.hex Firmware.vmem

echo "Cleanup and finish"
rm Firmware


