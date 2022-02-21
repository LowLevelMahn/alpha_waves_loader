@echo off

set my_root=F:\projects\fun\dos_games_rev
set tools_dir=%my_root%\tools
set org_dir=%my_root%\alpha_waves_dev\tests\alpha

set uasm_exe=%tools_dir%\uasm_x64\uasm64.exe

set WATCOM=%tools_dir%\open-watcom-2_0-c-win-x64
set WATCOM_BIN=%watcom%\binnt64
set INCLUDE=%watcom%\h
set PATH=%WATCOM_BIN%;%PATH%
set wlink_exe=%watcom_bin%\wlink.exe

%uasm_exe% ae.asm
if %ERRORLEVEL% EQU 0 goto next1
echo errorlevel %ERRORLEVEL%
pause
exit /b 1

:next1
%wlink_exe% name ae_org.com format dos com file ae.obj
if %ERRORLEVEL% EQU 0 goto next2
echo errorlevel %ERRORLEVEL%
pause
exit /b 1

:next2
fc /B %org_dir%\ALPHA_E.COM ae_org.COM
if %ERRORLEVEL% == 0 goto next3
echo !!!!
echo !!!! Resulting AE_ORG.COM is not binary identical to original ALPHA_E.COM !!!
echo !!!!
pause
exit /b 1

rem with reduced code down to start the game and exit

:next3
%uasm_exe% -DDIRECT_START -DCLEANUP ae.asm 
if %ERRORLEVEL% EQU 0 goto next4
echo errorlevel %ERRORLEVEL%
pause
exit /b 1

:next4
%wlink_exe% name ae.com format dos com file ae.obj
if %ERRORLEVEL% EQU 0 goto next5
echo errorlevel %ERRORLEVEL%
pause
exit /b 1

:next5
copy ae.com %org_dir%
if %ERRORLEVEL% EQU 0 goto next6
echo errorlevel %ERRORLEVEL%
pause
exit /b 1

:next6

pause
