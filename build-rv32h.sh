#!/bin/bash

# Source environment configuration.
. "config.sh"

# Generate instructions.
$TRAKTOR_HOME/bin/linux/releasestatic/Traktor.Run.App code/Instructions.template > code/Rv32H/Instructions.v

mkdir -p build/verilog
if iverilog -I code/Rv32H -o build/verilog/CPU_tb.out code/Rv32H/CPU_tb.v; then
	echo "Compile successful"
	vvp build/verilog/CPU_tb.out
else
	echo "-- Compile error --"
fi