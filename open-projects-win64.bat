@echo off

call %~dp0..\traktor\scripts\config.bat

start build\win64\Rv32.sln
