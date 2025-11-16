@echo off

setlocal enableextensions

set CURRENT_PATH="%~dp0"
set OPENOCD_TOOLS_PATH="C:\Program Files\Cypress\Cypress Programmer 4.2\openocd\bin"

if exist %OPENOCD_TOOLS_PATH% (
echo OPENOCD_TOOLS_PATH = %OPENOCD_TOOLS_PATH%
) else (
echo OpenOcd.exe does not exist.
goto:eof
)

cd /d %OPENOCD_TOOLS_PATH%

if exist "%1" (
openocd -s ../scripts -f interface/kitprog3.cfg -f target/psoc4.cfg -c "init; reset init; psoc4 mass_erase 0; shutdown"
openocd -s ../scripts -f interface/kitprog3.cfg -f target/psoc4.cfg -c "program %1 verify reset exit"
openocd -s ../scripts -f interface/kitprog3.cfg -f target/psoc4.cfg -c "init; reset run ;shutdown"
) else (
echo Enter the full pathname of the hex file.
echo Exe Example:
echo             Psoc4HexProgram.bat D:\psoc4-bridge-board-app_RELEASE.hex
)

cd /d %CURRENT_PATH%

pause
