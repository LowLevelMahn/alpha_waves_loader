@echo off

set my_root=F:\projects\fun\dos_games_rev
set tools_dir=%my_root%\tools
set org_dir=%my_root%\alpha_waves_dev\tests\alpha

set WATCOM=%tools_dir%\open-watcom-2_0-c-win-x64
set WATCOM_BIN=%watcom%\binnt64
set INCLUDE=%watcom%\h
set PATH=%WATCOM_BIN%;%PATH%

wasm.exe -wx -d1 loader.asm
if %ERRORLEVEL% EQU 0 goto next1
echo errorlevel %ERRORLEVEL%
pause
exit /b 1

:next1
wlink.exe DEBUG ALL name loader.com format dos com file loader.obj
if %ERRORLEVEL% EQU 0 goto next2
echo errorlevel %ERRORLEVEL%
pause
exit /b 1

:next2
copy loader.com %org_dir%
if %ERRORLEVEL% EQU 0 goto next3
echo errorlevel %ERRORLEVEL%
pause
exit /b 1

:next3

pause
