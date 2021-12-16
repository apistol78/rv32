#!/bin/bash

# Source environment configuration.
. "config.sh"

# Generate instructions.
$TRAKTOR_HOME/bin/linux/releasestatic/Traktor.Run.App code/Instructions.run verilog_inst_wire > code/Rv32H/Instructions_i.v
$TRAKTOR_HOME/bin/linux/releasestatic/Traktor.Run.App code/Instructions.run verilog_decode > code/Rv32H/Instructions_d.v

mkdir -p build/verilog
if iverilog -I code/Rv32H -o build/verilog/CPU_tb.out code/Rv32H/CPU_tb.v; then
	echo "Compile successful"
	vvp build/verilog/CPU_tb.out
else
	echo "-- Compile error --"
fi