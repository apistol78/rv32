@echo off

call %~dp0..\traktor\scripts\config.bat

%SOLUTIONBUILDER% ^
	-f=msvc ^
	-p=$(TRAKTOR_HOME)\resources\build\configurations\msvc-2022-win64.xml ^
	Rv32Win64.xms
