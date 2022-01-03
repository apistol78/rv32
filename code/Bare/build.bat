@echo off

echo Compiling...
riscv64-unknown-elf-gcc ^
    -O2 ^
    -march=rv32im ^
    -mabi=ilp32 ^
    -fdata-sections ^
    -ffunction-sections ^
    -Wl,--gc-sections ^
    -TLink.ld ^
    -IFatFs/source ^
    -o Bare ^
    CRC.c   ^
    File.c  ^
    GPIO.c  ^
    I2C.c   ^
    Main.c  ^
    Print.c ^
    SD.c    ^
    UART.c  ^
    Video.c ^
    FatFs/source/ff.c

echo Dumping...
riscv64-unknown-elf-objdump -D Bare > Bare.dump

echo Generating HEX image...
riscv64-unknown-elf-objcopy -O ihex Bare Bare.hex

echo Cleanup and finish
del Bare
