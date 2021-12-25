
:: . ../../config.sh

echo "Compiling..."
riscv64-unknown-elf-gcc -march=rv32im -mabi=ilp32 -O3 -TLink.ld -o Bare Bare.c 
:: Bare.s Bare.c

echo "Dumping..."
riscv64-unknown-elf-objdump -D Bare > Bare.dump

echo "Generating HEX image..."
riscv64-unknown-elf-objcopy -O ihex Bare Bare.hex

echo "Cleanup and finish"
del Bare
