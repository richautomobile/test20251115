@echo off
:start

setlocal enableextensions

set PROJECT_PATH=%cd%
:: cheng to linux path format
set PROJECT_PATH=%PROJECT_PATH:\=/%
set BUILDTIME=%date:~0,4%%date:~5,2%%date:~8,2%
:: Check CY_TOOLS_PATHS is defined
if "%CY_TOOLS_PATHS%"=="" (
    goto SetCyToolPathHelper
) 
:: Get the maximum number of parallel processes to run.
for /f "skip=1 delims== tokens=2" %%T in ('wmic cpu get NumberOfLogicalProcessors /value') do (
    set NUMCORE=%%T
)

:: change to DOS path format
set CYGWIN_SHELL=%CY_TOOLS_PATHS%/modus-shell/bin
set CYGWIN_SHELL=%CYGWIN_SHELL:/=\%


echo NUMBER_CORE = %NUMCORE%
echo CY_TOOLS_PATHS = %CY_TOOLS_PATHS%
echo PROJECT_PATH = %PROJECT_PATH%
echo CYGWIN_SHELL = %CYGWIN_SHELL%
cd /d %CYGWIN_SHELL%
%CYGWIN_SHELL%\bash.exe --login -c 'cd %PROJECT_PATH%; make CONFIG=Debug clean; make CONFIG=Debug CWD=%PROJECT_PATH% -j%NUMCORE% all;'
::%CYGWIN_SHELL%\bash.exe --login -c 'cd %PROJECT_PATH%; make CONFIG=Debug clean; make CONFIG=Debug CWD=%PROJECT_PATH% -j%NUMCORE% all;' > %PROJECT_PATH%/DebugBuild_Log.txt
::%CYGWIN_SHELL%\bash.exe --login -c "cd %PROJECT_PATH% && make CONFIG=Debug clean && make CONFIG=Debug CWD=%PROJECT_PATH% -j%NUMCORE% all" > %PROJECT_PATH%/DebugBuild_Log.txt

pause
goto:eof

:SetCyToolPathHelper
if exist "C:/Infineon/Tools/ModusToolbox/tools_3.0" (
    set CY_TOOLS_PATHS="C:/Infineon/Tools/ModusToolbox/tools_3.0"
)
if exist "C:/Infineon/Tools/ModusToolbox/tools_3.1" (
    set CY_TOOLS_PATHS="C:/Infineon/Tools/ModusToolbox/tools_3.1"
)

if "%CY_TOOLS_PATHS%"=="" (
    echo Can't automatically set CY_TOOLS_PATHS, please set CY_TOOLS_PATHS in env variable
)

goto:eof