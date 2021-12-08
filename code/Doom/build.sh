#!/bin/bash

. ../../config.sh

pushd doomgeneric
#make -f Makefile.rv32 clean
make -f Makefile.rv32
popd

riscv32-unknown-elf-objdump -D doomgeneric/fbdoom > fbdoom.dump
riscv32-unknown-elf-objcopy -O ihex doomgeneric/fbdoom fbdoom.hex
