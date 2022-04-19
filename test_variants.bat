@echo off

set dosbox_exe=F:\projects\fun\dos_games_rev\dosbox_dev\dosbox_vs2019\visualc_net\Release\dosbox.exe
set variants_dir=F:\projects\fun\dos_games_rev\alpha_waves_dev\tests\variants

echo CGA
%dosbox_exe% -conf %variants_dir%\dosbox-SVN.cga.conf
echo EGA
%dosbox_exe% -conf %variants_dir%\dosbox-SVN.ega.conf
echo TANDY
%dosbox_exe% -conf %variants_dir%\dosbox-SVN.tandy.conf
echo HERCULES
%dosbox_exe% -conf %variants_dir%\dosbox-SVN.hercules.conf
echo VGA
%dosbox_exe% -conf %variants_dir%\dosbox-SVN.vga.conf
