@echo off

echo Compiling...
riscv64-unknown-elf-gcc -O2 -march=rv32im -mabi=ilp32 -TLink.ld -o Bare ^
    CRC.c   ^
    GPIO.c  ^
    I2C.c   ^
    Main.c  ^
    Print.c ^
    SD.c    ^
    UART.c  ^
    Video.c

echo Dumping...
riscv64-unknown-elf-objdump -D Bare > Bare.dump

echo Generating HEX image...
riscv64-unknown-elf-objcopy -O ihex Bare Bare.hex

echo Cleanup and finish
del Bare
