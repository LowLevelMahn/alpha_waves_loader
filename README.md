# alpha_waves_loader

This is the reverse engineered, english-language text menu game loader ALPHA_E.COM of the 1990 DOS game Alpha Waves (aka Continuum) 

## What is a loader?

Due to super tiny floppy and memory sizes of that time most games used "loaders" that mostly combine graphics/sound/etc. code into in-memory build executables, ALPHA_E.COM is the english version of such a "loader" for AlphaWaves

## What else?

I've also developed a 32/64Bit tool for extracting and uncompressing the game executables and a 16Bit DOS starter program to skip the original loader

## Creators

Frederick Raynal did the DOS port im using here, the inital Ataris ST-Version of the game was created by Christophe de Dinechin

Loader menu | Game intro
--- | ---
![](./images/menu.png "Menu") | ![](./images/intro.png "Intro")

- Youtube video of the Atari-ST Vesion: https://www.youtube.com/watch?v=qwFPV855sI4
- Youtube video of the DOS Version: https://www.youtube.com/watch?v=jN9KKnfwoNY
- AlphaWaves on Moby Games: https://www.mobygames.com/game/dos/continuum
- Archive.org Floppy-Image download: https://archive.org/details/002297-AlphaWaves (my work bases on this floppy image, unpack disk image with 7zip)
- A bit of history on the game: https://grenouillebouillie.wordpress.com/2007/11/09/the-dawn-of-3d-games/
- Wikipedia entry: https://en.wikipedia.org/wiki/Alpha_Waves
- Modding Game Wiki Page: https://moddingwiki.shikadi.net/wiki/Alpha_Waves

| File         | MD5                               | Info                  | Analysed |
| :----------- | :-------------------------------- | :-------------------- | -------- |
| ALPHA_D.COM	 |  41c74f363e70864fb15579ba171731bf | german version        | ignored  |
| ALPHA_E.COM	 |  7e165fc5fd1aec1482bc915ab4053d36 | english version       | 100%      |
| ALPHA_F.COM	 |  682d26aec9512a4002d9aef271df0b23 | french version        | ignored  |
| CONFIG.TAT	 |  8b3de28f7feebc33e70b36c64061ab1f | gfx/game code offsets | 100% [see](read_some_file_sub_4/types.hpp)    |
| GRAPHS.CC1	 |  91d542578b974910ad30db6297bc8c0a | graphics data         | 5%       |
| HIGHSCOR.QB	 |  6fd61f2a03225192b7ac5a1dce5c96ea | highscores            | 100% [see](read_some_file_sub_4/types.hpp)    |
| MUSIC_A.CC1	 |  ab4eb457f274d7b28b6985662a1dc3b6 | adlib music           | 0%       |
| MUSIC_B.CC1	 |  cd968b9d80712830e1c2fa1cba170900 | buzer pc music        | 0%       |
| MUSIC_T.CC1	 |  8827fbc74d9053ff6363387a99be2f16 | tandy music           | 0%       |
| PROGS.CC1	   |  d529022d120dcd7bafedde05acd467c7 | gfx/game code         | 100% [see](read_some_file_sub_4/types.hpp)  |
| TATOU.BAT	   |  4a748577227640cbfd26874fe08c6d7c | simple starter        | ignored  |
| TESTFKEY.COM |	964a23b53713cfc666fbc081a72bf77b | ?                     | ignored  |
| TEXTES.CC1	 |  54393dca30917561ac33e4093096cc75 | multilang texts       | 1%       |

my reduced loader

![My loader](./images/loader.png "My loader")

# Files:

| File              | Info                                                                                                   |
| :---------------- | :----------------------------------------------------------------------------------------------------- |
| ALPHA_E.idb       | IDA 6.5x IDB of ALPHA_E.COM                                                                            |
| ae.asm            | reversed and reassemble-able source code of ALPHA_E.COM                                                |
| build.bat         | builds ae.asm with UASM64/WLink and checks if resulting ae_org.com is binary identical to ALPHA_E.COM  |
| tools_howto.txt   | how to get the needed build tools (UASM, WLink) - very easy to install                                 |
| exeload.asm       | small loader runs the extracted game exe and sound com TSR + set needed interrupts                     |
| build_exeload.bat | batch builder for the small loader                                                                     |
| read_some_file_sub_4/original_port.cpp  | C++ port of file loading routines based on a simple x86 emulator                 |
| read_some_file_sub_4/cleanup_port.cpp   | cleanup of the original port (mostly pure C/C++)                                 |
| tools/extractor/extractor.cpp  | standalone extractor for the game executables                                             |
| tools/starter/starter.cpp   | standalone 16Bit DOS starter for the extracted game executables                              |
| tools/create_static_exes/create_static_exes.cpp   | patches the ega_vga.exe to a vga.exe that starts without starter       |

 **tools_dir** variable in build.bat needs to be set to suits your environment

# Loading process

PROGS.CC1 contains the compressed executeable for:
  * Sound TSR COM-file-executable (installed at interrupt 0xF0)
    * Block[0] Adlib
    * Block[1] Tandy
    * Block[2] PC-Buz
  * Graphic-Mode related EXE-executables
    * Block[3] CGA/Hercules
    * Block[4] Tandy
    * Block[5] EGA/VGA

config.tat maps the gfx selection in the loader to the gfx executables in prog.cc1

| Mode         | Gfx                               |
| :----------: | :-------------------------------- |
|  0         	 | CGA/Hercules                      |
|  1           |EGA/VGA      |
|  2          |Tandy        |
|  3    |CGA/Hercules |
|  4    |EGA/VGA      |

all sound TSRs are loaded and excecuted in the order
1. adlib
2. tandy
3. pc-buz

(the first TSR that successfully hooks Interrupt 0xF0 keeps the others from installing)

the loader loads the executable that fits the gfx mode selection base on config.tat and progs.cc1
sets some interrupts and starts the game

# Extractor & Starter

under tools/uncompress_cc is a tool that can uncompress the CC1 files

the 16bit dos starter.exe creates the needed environment for the executables and starts the game

starter.exe starter [cga|ega|tandy|herc|vga] [adlib|tandy|pc|none]

create_static_exes.exe path-to-extracted-exes

 # using IDA
 1. add more information to the IDB
 2. produce ASM file in IDA (called ALPHA_E.asm)
 3. merge changes of ALPHA_E.asm into ae.asm
 4. test if resulting ae_org.com is equal to ALPHA_E.COM using build.bat

 # DONE
 - reverse/reassemble to binary equal com program
 - analysed the config.tat and HIGHSCOR.QB fully
 - documented the video-card detection, menu and config.tat reading code
 - created a reduced/cleanup version that gets rid of the video card detection and menu (parts NOPed out, unused data/code removed) which directly starts the game
 - binary indentical output test with MASM, UASM, WASM
 - extracted the pure VGA game exe (Turbo C 2.x) from the loading process
 - extracted the adlib TSR com program from the loading process (pre-loaded for adlib sound)
 - minimal loader for the adlib com TSR + game exe + setting of interrupts to configure the game (only GRAPHS.CC1, HIGHSCOR.QB, MUSIC_A.CC1, TEXTES.CC1 needed)
 - added a ultra-simple x86 "emulator" (based on inline-asm) that allows me to port loader code nearly 1:1 to C++, result: the game executables are load and uncompress-able in 32bit :)
 - executable-extractor and starter
 - create static vga.exe from extracted ega_vga.exe that does not need the starter to run
 - disabled copy protection - just press enter on code question when using ega.exe or vga.exe
 - reversed the CC1(CC0) compression format: https://moddingwiki.shikadi.net/wiki/CC1_Format and wrote a generic uncompress tool

 # TODOs
 - reverse the sound driver TSRs
 - document the file format of TEXTES.CC1, MUSIC_(A|B|T).CC1 and GRAPHS.CC1
 - patch port (03D0h as mirror of 03D4h) access to be more compatible with dosbox: https://github.com/dosbox-staging/dosbox-staging/issues/1448
 - document the Copy-Protection code stuff in the game (based on self modifying code)
 - maybe reverse AlphaWaves itself - its a Turbo C 2.x exe (Small memory model, 1 code segment, 1 data segment, the code-modifying protection makes it a litte hard to reverse)
 - remove copy protection code, replace self-modified code parts with static result - better for static analysis :)

 # Findings
 - VGA is not real VGA Mode but EGA 0dh (320x200x16) mode - its the same as game started with EGA but VGA color table change ability is used to lighten the colors (thanks @Mysterioso)

 # Related
 - [Alpha Waves, the first 3D platformer ever How 3D graphics worked when there were no graphic cards](https://www.youtube.com/watch?v=XMZj4WQ7Rt0)
 - [Full Atari ST source](http://cc3d.free.fr/Alpha-Waves.zip) - Build-environment: Devpac from [HiSoft Systems](https://en.wikipedia.org/wiki/HiSoft_Systems)
 - [GitHub account](https://github.com/c3d) and [GitHub page](https://c3d.github.io/) of Christophe de Dinechin
 - [Facebook Page](https://www.facebook.com/ludoid.raynal/) of Frederick Raynal's company
