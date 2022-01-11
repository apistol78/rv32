#!/bin/bash

# Source environment configuration.
. "config.sh"

# Generate instructions.
$TRAKTOR_HOME/bin/linux/releasestatic/Traktor.Run.App code/Instructions.run verilog_alu > code/Rv32H/Instructions_alu.v
$TRAKTOR_HOME/bin/linux/releasestatic/Traktor.Run.App code/Instructions.run verilog_ops > code/Rv32H/Instructions_ops.v
$TRAKTOR_HOME/bin/linux/releasestatic/Traktor.Run.App code/Instructions.run verilog_inst_wire > code/Rv32H/Instructions_decode.v
$TRAKTOR_HOME/bin/linux/releasestatic/Traktor.Run.App code/Instructions.run verilog_decode_ops > code/Rv32H/Instructions_decode_ops.v
$TRAKTOR_HOME/bin/linux/releasestatic/Traktor.Run.App code/Instructions.run verilog_execute_ops > code/Rv32H/Instructions_execute_ops.v

#mkdir -p build/verilog
#if iverilog -I code/Rv32H -D__ICARUS__ -D__TESTBENCH__ -o build/verilog/SoC_v2_tb.out code/Rv32H/SoC_v2_tb.v; then
#	echo "Compile successful"
#	vvp build/verilog/SoC_v2_tb.out
#else
#	echo "-- Compile error --"
#fi

mkdir -p build/verilator
verilator --cc -Icode/Rv32H -D__VERILATOR__ -D__TESTBENCH__ -Wno-WIDTH -Wno-WIDTHCONCAT --Mdir build/verilator --top-module SoC_v2_tb --build --exe code/Rv32H/SoC_v2_tb.v ../../code/SoC_v2_main.cpp
#verilator --cc -Icode/Rv32H -D__VERILATOR__ -D__TESTBENCH__ -Wno-WIDTH -Wno-WIDTHCONCAT --Mdir build/verilator --top-module SoC_v2_tb --build --exe code/Rv32H/SoC_v2_run.v ../../code/SoC_v2_run.cpp
