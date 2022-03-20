@echo off

set my_root=F:\projects\fun\dos_games_rev
set tools_dir=%my_root%\tools
set org_dir=%my_root%\alpha_waves_dev\tests\alpha

set uasm_exe=%tools_dir%\uasm_x64\uasm64.exe
set ulink_exe=%tools_dir%\ulink\ulink.exe

set WATCOM=%tools_dir%\open-watcom-2_0-c-win-x64
set WATCOM_BIN=%watcom%\binnt64
set INCLUDE=%watcom%\h
set PATH=%WATCOM_BIN%;%PATH%

::wasm.exe -wx loader.asm
%uasm_exe% -WX loader.asm
if %ERRORLEVEL% NEQ 0 goto error

%uasm_exe% -WX loader_data.asm
if %ERRORLEVEL% NEQ 0 goto error

wcc.exe loader_c.c -zl -zls -s -bt=dos -ms -nt=seg000
if %ERRORLEVEL% NEQ 0 goto error

::pure
::wlink.exe name loader.com format dos com file { loader.obj } option MANGlednames

::data segment only
::wlink.exe name loader.com format dos com file { loader.obj loader_data.obj } option MANGlednames

::data segment+c
wlink.exe name loader.com format dos com file { loader.obj loader_c.obj loader_data.obj } option MANGlednames
if %ERRORLEVEL% NEQ 0 goto error

copy loader.com %org_dir%
if %ERRORLEVEL% NEQ 0 goto error

exit /b 0

:error
echo errorlevel %ERRORLEVEL%
pause
exit /b 1
