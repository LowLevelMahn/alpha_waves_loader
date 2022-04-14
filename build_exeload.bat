@echo off

set my_root=F:\projects\fun\dos_games_rev
set tools_dir=%my_root%\tools
set org_dir=%my_root%\alpha_waves_dev\tests\alpha

set uasm_exe=%tools_dir%\uasm_x64\uasm64.exe
set ulink_exe=%tools_dir%\ulink\ulink.exe
set exehdr_exe=%tools_dir%\exetools\exehdr.exe

set WATCOM=%tools_dir%\open-watcom-2_0-c-win-x64
set WATCOM_BIN=%watcom%\binnt64
set INCLUDE=%watcom%\h
set PATH=%WATCOM_BIN%;%PATH%

del exeload.obj exeload.exe

%uasm_exe% -WX exeload.asm
echo %ERRORLEVEL%
if %ERRORLEVEL% NEQ 0 goto error

%ulink_exe% -Tde -p- exeload.obj
echo %ERRORLEVEL%
if %ERRORLEVEL% NEQ 0 goto error

copy exeload.exe %org_dir%
echo %ERRORLEVEL%
if %ERRORLEVEL% NEQ 0 goto error

pause
exit /b 0

:error
echo errorlevel %ERRORLEVEL%
pause
exit /b 1
