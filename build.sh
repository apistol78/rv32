#!/bin/bash

# Source environment configuration.
. "scripts/config.sh"

# Generate instructions (verilog).
$TRAKTOR_HOME/bin/linux/releasestatic/Traktor.Run.App code/Instructions.run verilog_alu > rtl/cpu/private/generated/Instructions_alu.sv
$TRAKTOR_HOME/bin/linux/releasestatic/Traktor.Run.App code/Instructions.run verilog_memory > rtl/cpu/private/generated/Instructions_memory.sv
$TRAKTOR_HOME/bin/linux/releasestatic/Traktor.Run.App code/Instructions.run verilog_ops > rtl/cpu/private/generated/Instructions_ops.sv
$TRAKTOR_HOME/bin/linux/releasestatic/Traktor.Run.App code/Instructions.run verilog_decode > rtl/cpu/private/generated/Instructions_decode.sv
$TRAKTOR_HOME/bin/linux/releasestatic/Traktor.Run.App code/Instructions.run verilog_decode_ops > rtl/cpu/private/generated/Instructions_decode_ops.sv
$TRAKTOR_HOME/bin/linux/releasestatic/Traktor.Run.App code/Instructions.run verilog_execute_ops I J R U B S CSR > rtl/cpu/private/generated/Instructions_execute_ops.sv
#$TRAKTOR_HOME/bin/linux/releasestatic/Traktor.Run.App code/Instructions.run verilog_fpu > rtl/cpu/private/generated/Instructions_fpu.sv

# Generate instructions (emulator).
$TRAKTOR_HOME/bin/linux/releasestatic/Traktor.Run.App code/Instructions.run cpp > code/Rv32/Instructions.inl

# Build target.
./scripts/build-projects-make-rv32.sh
pushd build/rv32
make -j8 -f Rv32.mak ReleaseStatic
popd

# Generate firmware verilog memory file.
riscv32-unknown-elf-objcopy -O ihex build/rv32/ReleaseStatic/Firmware Firmware.hex
bin/Hex2Verilog -word=32 Firmware.hex -vmem=Firmware.vmem -vmem-range=Firmware.vmem-range
rm Firmware.hex

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

# Build host.
./scripts/build-projects-make-linux.sh
pushd build/linux
make -j8 -f Rv32.mak ReleaseStatic
popd

# Generate useful dumps.
mkdir -p dumps
riscv32-unknown-elf-objdump -D build/rv32/ReleaseStatic/Firmware > dumps/Firmware.dump
riscv32-unknown-elf-objdump -x build/rv32/ReleaseStatic/Firmware > dumps/Firmware.map
riscv32-unknown-elf-objdump -D build/rv32/ReleaseStatic/Boot > dumps/Boot.dump
riscv32-unknown-elf-objdump -x build/rv32/ReleaseStatic/Boot > dumps/Boot.map
riscv32-unknown-elf-objdump -D build/rv32/ReleaseStatic/Doom > dumps/Doom.dump
riscv32-unknown-elf-objdump -D build/rv32/ReleaseStatic/Quake > dumps/Quake.dump
riscv32-unknown-elf-objdump -D build/rv32/ReleaseStatic/ScummRV > dumps/ScummRV.dump
