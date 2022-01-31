#!/bin/bash

# Source environment configuration.
. "config.sh"

# Generate instructions (Verilog).
$TRAKTOR_HOME/bin/linux/releasestatic/Traktor.Run.App code/Instructions.run verilog_alu > code/Rv32H/Instructions_alu.v
$TRAKTOR_HOME/bin/linux/releasestatic/Traktor.Run.App code/Instructions.run verilog_memory > code/Rv32H/Instructions_memory.v
$TRAKTOR_HOME/bin/linux/releasestatic/Traktor.Run.App code/Instructions.run verilog_ops > code/Rv32H/Instructions_ops.v
$TRAKTOR_HOME/bin/linux/releasestatic/Traktor.Run.App code/Instructions.run verilog_inst_wire > code/Rv32H/Instructions_decode.v
$TRAKTOR_HOME/bin/linux/releasestatic/Traktor.Run.App code/Instructions.run verilog_decode_ops > code/Rv32H/Instructions_decode_ops.v
$TRAKTOR_HOME/bin/linux/releasestatic/Traktor.Run.App code/Instructions.run verilog_execute_ops I J R U B S CSR > code/Rv32H/Instructions_execute_ops.v

# Generate instructions (Emulator).
$TRAKTOR_HOME/bin/linux/releasestatic/Traktor.Run.App code/Instructions.run cpp > code/Rv32/Instructions.inl

# Generate verilated code.
mkdir -p code/Verify/SoC
verilator --trace --cc -Icode/Rv32H -D__VERILATOR__ -D__TESTBENCH__ -Wno-WIDTH -Wno-WIDTHCONCAT --Mdir code/Verify/SoC --top-module SoC code/Rv32H/SoC.v
if [ $? -ne 0 ]; then
	exit 1
fi

mkdir -p code/Rv32T/SoC
verilator --trace --cc -Icode/Rv32H -D__VERILATOR__ -D__TESTBENCH__ -Wno-WIDTH -Wno-WIDTHCONCAT --Mdir code/Rv32T/SoC --top-module SoC code/Rv32H/SoC.v
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
riscv32-unknown-elf-objdump -D build/rv32/ReleaseStatic/Bare > Bare.dump
riscv32-unknown-elf-objdump -D build/rv32/ReleaseStatic/Doom > Doom.dump