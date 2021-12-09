#!/bin/sh

if iverilog -o ap_spi_master_cpu_tb.out ap_spi_master_cpu_tb.v; then
	vvp ap_spi_master_cpu_tb.out
else
	echo "-- Compile error --"
fi
