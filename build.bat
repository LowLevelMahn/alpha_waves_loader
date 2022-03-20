@echo off

set my_root=F:\projects\fun\dos_games_rev
set tools_dir=%my_root%\tools
set org_dir=%my_root%\alpha_waves_dev\tests\alpha

set uasm_exe=%tools_dir%\uasm_x64\uasm64.exe
:: latest 16bit able ML 9 from VS2008 Express
set ml_exe=%tools_dir%\masm\9\ml.exe 

set WATCOM=%tools_dir%\open-watcom-2_0-c-win-x64
set WATCOM_BIN=%watcom%\binnt64
set INCLUDE=%watcom%\h
set PATH=%WATCOM_BIN%;%PATH%
set wlink_exe=%watcom_bin%\wlink.exe

:: binary equal build tests

:: im building with multiple assembler to detect bugs in the tool-chain (every assembler gots its own micro problems)

:: ==================================================
:: MASM/ML
:: ==================================================

%ml_exe% /c /omf /DBINARY_EQUAL ae.asm
if %ERRORLEVEL% NEQ 0 goto error

%wlink_exe% name ae_org_m.com format dos com file ae.obj
if %ERRORLEVEL% NEQ 0 goto error

fc /B %org_dir%\ALPHA_E.COM ae_org_m.COM
if %ERRORLEVEL% NEQ 0 goto error

:: ==================================================
:: UASM
:: ==================================================

%uasm_exe% -WX -DBINARY_EQUAL ae.asm
if %ERRORLEVEL% NEQ 0 goto error

%wlink_exe% name ae_org_u.com format dos com file ae.obj
if %ERRORLEVEL% NEQ 0 goto error

fc /B %org_dir%\ALPHA_E.COM ae_org_u.COM
if %ERRORLEVEL% NEQ 0 goto error

:: ==================================================
:: WASM
:: ==================================================

::!!! TODO: make binary compatible !!!

::wasm.exe -wx -dBINARY_EQUAL ae.asm
::if %ERRORLEVEL% NEQ 0 goto error
::
::%wlink_exe% name ae_org_w.com format dos com file ae.obj
::if %ERRORLEVEL% NEQ 0 goto error
::
::fc /B %org_dir%\ALPHA_E.COM ae_org_w.COM
::if %ERRORLEVEL% NEQ 0 goto error

:: ==================================================
:: with reduced code down to just start the game and exit, binary equality not needed here
:: ==================================================

::%uasm_exe% -DDIRECT_START -DCLEANUP ae.asm 
%uasm_exe% -DREMOVE_DEAD_CODE ae.asm 
if %ERRORLEVEL% NEQ 0 goto error

%wlink_exe% name ae.com format dos com file ae.obj
if %ERRORLEVEL% NEQ 0 goto error

copy ae.com %org_dir%
if %ERRORLEVEL% NEQ 0 goto error

exit /b 0

:error
pause
exit /b 1

