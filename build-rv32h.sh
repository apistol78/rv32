#!/bin/bash

# Source environment configuration.
. "config.sh"

# Generate instructions.
$TRAKTOR_HOME/bin/linux/releasestatic/Traktor.Run.App code/Instructions.template > code/Rv32H/Instructions.v

if iverilog -I code/Rv32H -o build/verilog/CPU.out code/Rv32H/CPU.v; then
	#vvp ap_cpu_tb.out
	echo "Compile succesful"
else
	echo "-- Compile error --"
fi