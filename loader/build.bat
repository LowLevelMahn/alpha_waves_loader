@echo off

rem ASSEMBLER=WASM,UASM,MASM
rem COMPILER=WCC,DMC(,BCC)
rem LINKER=WLINK,ULINK,OPTLINK

rem set use_uasm=1
rem set use_masm=0
rem set use_wasm=0

rem set use_wcc=1
rem set use_dmc=0

rem set use_wlink=1
rem set use_ulink=0
rem set use_optlink=0

rem if %use_masm% equ 1 (
rem ...
rem )

set my_root=F:\projects\fun\dos_games_rev
set org_dir=%my_root%\alpha_waves_dev\tests\alpha

set tools_dir=%my_root%\tools
set ulink_exe=%tools_dir%\ulink\ulink.exe
set uasm_exe=%tools_dir%\uasm_x64\uasm64.exe

set WATCOM=%tools_dir%\open-watcom-2_0-c-win-x64
set WATCOM_BIN=%watcom%\binnt64
set INCLUDE=%watcom%\h
set PATH=%WATCOM_BIN%;%PATH%
set wlink_exe=%watcom_bin%\wlink.exe
set wcc_exe=%watcom_bin%\wcc.exe
set wcl_exe=%watcom_bin%\wcl.exe
set wasm_exe=%watcom_bin%\wasm.exe

echo =======================
echo "c_part.exe"

%wcc_exe% c_part.c -2 -bt=dos -dWITH_MAIN
%wlink_exe% name wc_cp.exe system dos file { c_part.obj }
copy wc_cp.exe %org_dir%

echo =======================
pause

::goto ende

:: masm 6.15 supports .286
:: https://sites.google.com/site/pcdosretro/masmhist
:: Version 9 (VS2008) seems to be latest with 16bit support, Version 10 (VS2010) dropped 16-bit assembly support
:: Version 9 needs /omf, Version 6.15 has omf as only output
:: Version 10 does not support .286

::set PATH=%PATH%;%tools_dir%\masm\6.15\bin;%PATH%
set PATH=%PATH%;%tools_dir%\masm\9

ml.exe /c /omf main.asm
if %ERRORLEVEL% EQU 0 goto next1
echo errorlevel %ERRORLEVEL%
pause
exit /b 1

:next1

ml.exe /c /omf asm_part.asm
if %ERRORLEVEL% EQU 0 goto next2
echo errorlevel %ERRORLEVEL%
pause
exit /b 1

:next2
goto next4

%wasm_exe% main.asm -mt -bt=dos
::%uasm_exe% main.asm -mt -bt=dos
if %ERRORLEVEL% EQU 0 goto next3
echo errorlevel %ERRORLEVEL%
pause
exit /b 1

:next3

%wasm_exe% asm_part.asm -mt -bt=dos
::%uasm_exe% asm_part.asm
if %ERRORLEVEL% EQU 0 goto next4
echo errorlevel %ERRORLEVEL%
pause
exit /b 1

:next4

set DMC_DIR=%tools_dir%\dm857c\dm
set DMC_BIN_DIR=%DMC_DIR%\bin
set INCLUDE=%DMC_DIR%\include
SET LIB=%DMC_DIR%\lib;%LIB%
set PATH=%DMC_BIN_DIR%;%PATH%

pause
dmc -odmc_cp.exe c_part.c -2 -ms -DWITH_MAIN
copy dmc_cp.exe %org_dir%

rem -nt=seg000 renames _TEXT to seg000 make it cleanly combineable with wlink
rem %wcc_exe% c_part.c -2 -zl -zls -s -bt=dos -ms -nt=seg000
dmc -NL -NTseg000 -c -2 -ms c_part.c 
pause

if %ERRORLEVEL% EQU 0 goto next5
echo errorlevel %ERRORLEVEL%
pause
exit /b 1

:next5

::%wlink_exe% name main.com format dos com file { main.obj c_part.obj asm_part.obj }  option { map=loader.map }
%ulink_exe% -Tdc main.obj c_part.obj asm_part.obj
if %ERRORLEVEL% EQU 0 goto next6
echo errorlevel %ERRORLEVEL%
pause
exit /b 1

:next6
copy main.com %org_dir%
if %ERRORLEVEL% EQU 0 goto next7
echo errorlevel %ERRORLEVEL%
pause
exit /b 1

:next7

exit /b 0

:ende