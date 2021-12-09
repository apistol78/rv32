#!/bin/sh

if iverilog -o ap_cpu_tb.out ap_cpu_tb.v; then
	vvp ap_cpu_tb.out
else
	echo "-- Compile error --"
fi
