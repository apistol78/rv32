@echo off

:: Get game home path using some bat magic.
for %%i in ("%~dp0.") do (set GAME_HOME=%%~dpi)
if %GAME_HOME:~-1%==\ set GAME_HOME=%GAME_HOME:~0,-1%

:: Setup traktor home also.
call "%GAME_HOME%\traktor\scripts\config.bat"
