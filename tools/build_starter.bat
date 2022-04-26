@echo off

set my_root=F:\projects\fun\dos_games_rev
set tools_dir=%my_root%\tools

set WATCOM=%tools_dir%\open-watcom-2_0-c-win-x64
set WATCOM_BIN=%watcom%\binnt64
set INCLUDE=%watcom%\h
set PATH=%WATCOM_BIN%;%PATH%

del starter.obj starter.exe

wcl.exe -bt=dos starter.cpp 
echo %ERRORLEVEL%
if %ERRORLEVEL% NEQ 0 goto error

pause
exit /b 0

:error
echo errorlevel %ERRORLEVEL%
pause
exit /b 1
