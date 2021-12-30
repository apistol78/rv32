@echo off

:: Setup environment.
call %~dp0config.bat

:: Generate instructions.
"%TRAKTOR_HOME%\bin\win64\releasestatic\Traktor.Run.App" code/Instructions.run verilog_inst_wire > code/Rv32H/Instructions_i.v
"%TRAKTOR_HOME%\bin\win64\releasestatic\Traktor.Run.App" code/Instructions.run verilog_decode > code/Rv32H/Instructions_d.v
"%TRAKTOR_HOME%\bin\win64\releasestatic\Traktor.Run.App" code/Instructions.run verilog_alu > code/Rv32H/Instructions_alu.v

mkdir build\verilog
:: iverilog -I code/Rv32H -D__ICARUS__ -o build/verilog/SoC_tb.out code/Rv32H/SoC_tb.v
:: vvp build/verilog/SoC_tb.out

iverilog -I code/Rv32H -D__ICARUS__ -o build/verilog/SoC_v2_tb.out code/Rv32H/SoC_v2_tb.v
if errorlevel 1 exit

vvp build/verilog/SoC_v2_tb.out
