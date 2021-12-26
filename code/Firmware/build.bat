
:: . ../../config.sh

echo "Compiling..."
riscv64-unknown-elf-gcc -march=rv32im -mabi=ilp32 -nostdlib -ffreestanding -TLink.ld -o Firmware Startup.s Firmware.c

echo "Dumping..."
riscv64-unknown-elf-objdump -D Firmware > Firmware.dump

echo "Generating HEX image..."
riscv64-unknown-elf-objcopy -O ihex Firmware Firmware.hex

echo "Converting image into Verilog flat image..."
..\..\build\win64\ReleaseStatic\Hex2Verilog -word=32 Firmware.hex Firmware.vmem

echo "Cleanup and finish"
del Firmware
