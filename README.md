# alpha_waves_loader

This is the reverse engineered, english-language text menu game loader ALPHA_E.COM of the DOS game Alpha Waves aka Continuum

Due to super tiny floppy and memory sizes of that time most games used "loaders" that mostly combine graphics/sound/etc. code into in-memory build executables, ALPHA_E.COM is the english version of such a "loader" for AlphaWaves

Loader menu | Game intro
--- | ---
![](./images/menu.png "Menu") | ![](./images/intro.png "Intro")
 
- Youtube: https://www.youtube.com/watch?v=jN9KKnfwoNY
- AlphaWaves on Moby Games: https://www.mobygames.com/game/dos/continuum
- Archive.org Floppy-Image download: https://archive.org/details/002297-AlphaWaves (my work bases on this floppy image, unpack disk image with 7zip)

| File         | MD5                               | Info            |                                                  
| :----------- | :-------------------------------- | :-------------- |
| ALPHA_D.COM	 |  41c74f363e70864fb15579ba171731bf | german version  | 
| ALPHA_E.COM	 |  7e165fc5fd1aec1482bc915ab4053d36 | english version |
| ALPHA_F.COM	 |  682d26aec9512a4002d9aef271df0b23 | french version  |
| CONFIG.TAT	  |  8b3de28f7feebc33e70b36c64061ab1f | gfx/game code offsets |
| GRAPHS.CC1	  |  91d542578b974910ad30db6297bc8c0a | graphics data   |
| HIGHSCOR.QB	 |  6fd61f2a03225192b7ac5a1dce5c96ea | highscores      |
| MUSIC_A.CC1	 |  ab4eb457f274d7b28b6985662a1dc3b6 | music           |
| MUSIC_B.CC1	 |  cd968b9d80712830e1c2fa1cba170900 | music           |
| MUSIC_T.CC1	 |  8827fbc74d9053ff6363387a99be2f16 | music           |
| PROGS.CC1	   |  d529022d120dcd7bafedde05acd467c7 | gfx/game code   |
| TATOU.BAT	   |  4a748577227640cbfd26874fe08c6d7c | simple starter  |
| TESTFKEY.COM |	 964a23b53713cfc666fbc081a72bf77b | ?               |
| TEXTES.CC1	  |  54393dca30917561ac33e4093096cc75 | multilang texts |

my reduced loader

![My loader](./images/loader.png "My loader")

# Files:

| File              | Info                                                                                                   |
| :---------------- | :----------------------------------------------------------------------------------------------------- |
| ALPHA_E.idb       | IDA 6.5x IDB of ALPHA_E.COM                                                                            |
| ae.asm            | reversed and reassemble-able source code of ALPHA_E.COM                                                |
| build.bat         | builds ae.asm with UASM64/WLink and checks if resulting ae_org.com is binary identical to ALPHA_E.COM  |
| tools_howto.txt   | how to get the needed build tools (UASM, WLink) - very easy to install                                 |
| ae_org.com        | ae.asm build as-is - is binary identical to ALPHA_E.com (checked by build.bat)                         |
| ae.com            | ae.asm build with DIRECT_START, direct start of the game, no menu, most code NOPed out                 |

 **tools_dir** variable in build.bat needs to be set to suits your environment
 
 # using IDA
 1. add more information to the IDB
 2. produce ASM file in IDA (called ALPHA_E.asm)
 3. merge changes of ALPHA_E.asm into ae.asm
 4. test if resulting ae_org.com is equal to ALPHA_E.COM using build.bat
 
 # DONE
 - reverse/reassemble to binary equal com program
 - analysed the config.tat and HIGHSCOR.QB fully
 - documented the video-card detection, menu and config.tat reading code
 - created a reduced version that gets rid of the video card detection and menu (NOPed out) which directly starts the game
 
 # TODOs
 - there are still non-symbolic offsets or something in the game-start code - the loader crashes if i remove the NOPed parts completely
 - game-starting analyse is in the very beginning: so far: an exe gets created from parts of progs1.cc in ram and executed 
 - rewrite the loader in C and write a tool that directly creates a full game exe that can be directly started (i know i can just memdump, but thats not my goal)
 - maybe reverse AlphaWaves itself - its a Turbo C 2.x exe
