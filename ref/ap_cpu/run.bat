@echo off

iverilog -o ap_cpu_tb.out ap_cpu_tb.v
if %ERRORLEVEL% equ 0 (
	vvp ap_cpu_tb.out
)
