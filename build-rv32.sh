#!/bin/bash

# Source environment configuration.
. "config.sh"

# Generate instructions.
$TRAKTOR_HOME/bin/linux/releasestatic/Traktor.Run.App code/Instructions.run cpp > code/Rv32/Instructions.inl

# Build emulator.
pushd build/linux
make -f Rv32.mak ReleaseStatic
popd

# Build target.
pushd build/rv32
make -f Rv32.mak ReleaseStatic
popd

# Generate HEX images.
riscv32-unknown-elf-objcopy -O ihex build/rv32/ReleaseStatic/Bare Bare.hex
riscv32-unknown-elf-objcopy -O ihex build/rv32/ReleaseStatic/Doom Doom.hex

# Generate useful dumps.
riscv32-unknown-elf-objdump -D build/rv32/ReleaseStatic/Bare > Bare.dump
riscv32-unknown-elf-objdump -D build/rv32/ReleaseStatic/Doom > Doom.dump