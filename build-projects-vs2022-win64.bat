@echo off

call %~dp0..\traktor\scripts\config.bat

:: Generate instructions.
"%TRAKTOR_HOME%\bin\win64\releasestatic\Traktor.Run.App" code/Instructions.run cpp > code/Rv32/Instructions.inl

%SOLUTIONBUILDER% ^
	-f=msvc ^
	-p=$(TRAKTOR_HOME)\resources\build\configurations\msvc-2022-win64.xml ^
	Rv32Win64.xms
