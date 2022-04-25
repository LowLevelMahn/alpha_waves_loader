# alpha_waves_loader

This is the reverse engineered, english-language text menu game loader ALPHA_E.COM of the DOS game Alpha Waves aka Continuum

Due to super tiny floppy and memory sizes of that time most games used "loaders" that mostly combine graphics/sound/etc. code into in-memory build executables, ALPHA_E.COM is the english version of such a "loader" for AlphaWaves

Loader menu | Game intro
--- | ---
![](./images/menu.png "Menu") | ![](./images/intro.png "Intro")
 
- Youtube: https://www.youtube.com/watch?v=jN9KKnfwoNY
- AlphaWaves on Moby Games: https://www.mobygames.com/game/dos/continuum
- Archive.org Floppy-Image download: https://archive.org/details/002297-AlphaWaves (my work bases on this floppy image, unpack disk image with 7zip)

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

 **tools_dir** variable in build.bat needs to be set to suits your environment

# Loading process

progs.cc1 contains the compressed executeable for:
  * sound com TSR (installed at interrupt 0xF0)
    * adlib
    * tandy
    * pc-buz
  * gfx exe
    * cga/hercules
    * tandy
    * ega/vga
    
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

 # TODOs
 - rewrite the loader in C and write a tool that directly creates a full game exe that can be directly started (i know i can just memdump, but thats not my goal)
 - maybe reverse AlphaWaves itself - its a Turbo C 2.x exe
 
