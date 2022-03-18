#!/bin/bash

# Source environment configuration.
. "config.sh"

# Generate instructions (Verilog).
$TRAKTOR_HOME/bin/linux/releasestatic/Traktor.Run.App code/Instructions.run verilog_alu > rtl/cpu/private/generated/Instructions_alu.sv
$TRAKTOR_HOME/bin/linux/releasestatic/Traktor.Run.App code/Instructions.run verilog_memory > rtl/cpu/private/generated/Instructions_memory.sv
$TRAKTOR_HOME/bin/linux/releasestatic/Traktor.Run.App code/Instructions.run verilog_ops > rtl/cpu/private/generated/Instructions_ops.sv
$TRAKTOR_HOME/bin/linux/releasestatic/Traktor.Run.App code/Instructions.run verilog_decode > rtl/cpu/private/generated/Instructions_decode.sv
$TRAKTOR_HOME/bin/linux/releasestatic/Traktor.Run.App code/Instructions.run verilog_decode_ops > rtl/cpu/private/generated/Instructions_decode_ops.sv
$TRAKTOR_HOME/bin/linux/releasestatic/Traktor.Run.App code/Instructions.run verilog_execute_ops I J R U B S CSR > rtl/cpu/private/generated/Instructions_execute_ops.sv
$TRAKTOR_HOME/bin/linux/releasestatic/Traktor.Run.App code/Instructions.run verilog_fpu > rtl/cpu/private/generated/Instructions_fpu.sv

# Generate instructions (Emulator).
$TRAKTOR_HOME/bin/linux/releasestatic/Traktor.Run.App code/Instructions.run cpp > code/Rv32/Instructions.inl

# Generate verilated code.
mkdir -p code/Verify/SoC
verilator --trace-fst --trace-structs --cc -Irtl/cpu -Irtl/cpu/private -Irtl/gpu -Irtl/peripherials -D__VERILATOR__ -D__TESTBENCH__ -Wno-WIDTH -Wno-WIDTHCONCAT -Wno-TIMESCALEMOD --Mdir code/Verify/SoC --top-module SoC board/verify/rtl/SoC.sv
if [ $? -ne 0 ]; then
	exit 1
fi

mkdir -p code/Rv32T/SoC
verilator --trace-fst --trace-structs --cc -Irtl/cpu -Irtl/cpu/private -Irtl/gpu -Irtl/peripherials -D__VERILATOR__ -D__TESTBENCH__ -Wno-WIDTH -Wno-WIDTHCONCAT -Wno-TIMESCALEMOD --Mdir code/Rv32T/SoC --top-module SoC board/rv32t/rtl/SoC.sv
if [ $? -ne 0 ]; then
	exit 1
fi

# Generate solutions.
./build-projects-make-linux.sh
./build-projects-make-rv32.sh

# Build host projects.
pushd build/linux
#make -j4 -f Rv32.mak clean
make -j4 -f Rv32.mak ReleaseStatic
popd

# Build target projects.
pushd build/rv32
make -j4 -f Rv32.mak ReleaseStatic
popd

# Generate useful dumps.
riscv32-unknown-elf-objdump -D build/rv32/ReleaseStatic/Firmware > Firmware.dump
riscv32-unknown-elf-objdump -D build/rv32/ReleaseStatic/Bare > Bare.dump
riscv32-unknown-elf-objdump -D build/rv32/ReleaseStatic/Doom > Doom.dump
riscv32-unknown-elf-objdump -D build/rv32/ReleaseStatic/Quake > Quake.dump

# Generate firmware verilog memory file.
riscv32-unknown-elf-objcopy -O ihex build/rv32/ReleaseStatic/Firmware Firmware.hex
build/linux/ReleaseStatic/Hex2Verilog -word=32 Firmware.hex Firmware.vmem
rm Firmware.hex
