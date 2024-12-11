#include <stdlib.h>
#include <process.h>
#include <stdio.h>
#include <dos.h>
#include <stdint.h>
#include <i86.h>
#include <string.h>

struct far_ptr_t
{
  uint16_t offs;
  uint16_t segm;
};

bool get_string_index(const char* str_, const char* list_[], int list_size_, uint16_t* str_index_)
{
  *str_index_ = -1;
  for(int i = 0; i < list_size_; ++i)
  {
    if(strcmp(list_[i], str_) == 0)
    {
      *str_index_ = i;
      return true;
    }
  }
  return false;
}

#define COUNTOF(ARRAY) (sizeof(ARRAY) / sizeof(ARRAY[0]))

bool parse_cmd(int argc_, char* argv_[], uint16_t* gfx_mode_, uint16_t* sound_type_)
{
  if( argc_ < 3)
  {
    return false;
  }

  *gfx_mode_ = 0;
  static const char* gfx_str[5] = {"cga","ega","tandy","herc","vga"};
  if(!get_string_index(argv_[1], gfx_str, COUNTOF(gfx_str), gfx_mode_))
  {
    printf("unknown gfx: %s\n", argv_[1]);
    return false;    
  }
  
  *sound_type_ = 0;
  static const char* sound_str[4] = {"adlib","tandy","pc","none"};
  if(!get_string_index(argv_[2], sound_str, COUNTOF(sound_str), sound_type_))
  {
    printf("unknown sound: %s\n", argv_[2]);
    return false;    
  }
  
  return true;
}

const char* CGA_HERC = "cga_herc.exe";
const char* EGA_VGA = "ega_vga.exe";
const char* TANDY = "tandy.exe";
const char* game_exec[5] = {CGA_HERC, EGA_VGA, TANDY, CGA_HERC, EGA_VGA};

const char* sound_exec[3] = {"s_adlib.com","s_tandy.com","s_pc_buz.com"};

uint8_t bool_as_int(bool value_)
{
  return value_ ? 1 : 0;
}

uint16_t get_feature_flags( bool unknown_, bool joystick_support_, bool currency_franc_, uint8_t free_memory_type_ )
{
/*
feature_flags
  bit[ 0] = 1 ???
  bit[ 1] = 0
  bit[ 2] = 0
  bit[ 3] = 0
  bit[ 4] = 0
  bit[ 5] = 0
  bit[ 6] = 0
  bit[ 7] = 0
  bit[ 8] = 0
  bit[ 9] = 0
  bit[ 10] = 0
  bit[ 11] = 0
  bit[ 12] = joystick detected (not supported currently)
  bit[ 13] = currency == franc
  bit[14+15] = free memory on game start
    = 0b00 = 0 ( < 0x4000 free paragraphs)
    = 0b01 = 1 (>= 0x4000 free paragraphs)
    = 0b10 = 2 (>= 0x6000 free paragraphs)
    = 0b11 = 3 (>= 0x8000 free paragraphs)
*/
  uint16_t feature_flags = 
    (free_memory_type_ << 14) 
    + (bool_as_int(currency_franc_) << 13) 
    + (bool_as_int(joystick_support_) << 12) 
    + bool_as_int(unknown_);
    
  return feature_flags;
}

//#define SET_INT_24h

#if SET_INT_24h

const uint8_t dos_version = 3;

// never called? in Sound-Driver?
// dosbox debugger bpint 24 does not stop anywhere
// no 0xCD 0x24 opcode in executables (only as data)
// gets overrwritten by the game (interrupt-table changes from start to end at interrupt 24h)
// bpint 21 4c
// at file offset 0x5802 (seg000:5602) in ega_vga.exe get set with offset/segment
// same routine that calls interrupt 97h
void __interrupt __far interrupt_24h(INTPACK regs_)
{
  regs_.h.al = dos_version; // returns always 3 if dos version is >= 3 (dosbox is version 5)
}

#endif

uint16_t feature_flags = 0;

// called in ega_vga.exe from file offset: 0x57DA
void __interrupt __far interrupt_97h(INTPACK regs_)
{
  regs_.x.ax = feature_flags;
  
  //word_24F76 = ax & 0xC000 (0b1100000000000000)  // free memory type
  //word_24F70 = ax & 0x2000 (0b0010000000000000)  // franc is currency
  //word_24F72 = ax & 0x1000 (0b0001000000000000)  // joystick
}

typedef void __interrupt __far (*interrupt_func)();

int main(int argc_, char* argv_[])
{
  feature_flags = get_feature_flags(true, false, false, 3);

  uint16_t gfx_mode = 0;
  uint16_t sound_type = 0;
  
  if(!parse_cmd(argc_, argv_, &gfx_mode, &sound_type))
  {
    printf("command line parameters invalid\n");
    printf("\n");
    printf("  starter [gfx] [sound] (case sensitive)\n");
    printf("  starter cga|ega|tandy|herc|vga adlib|tandy|pc|none\n");
    printf("\n");
    printf("  example:\n");
    printf("  starter ega pc\n");
    printf("  starter vga adlib\n");
    printf("\n");
    return 1;
  }

  // save&set interrupts

#if SET_INT_24h
  interrupt_func old_int_0x24 = _dos_getvect( 0x24 );  
#endif  
  interrupt_func old_int_0x97 = _dos_getvect( 0x97 );
  interrupt_func old_int_0x9F = _dos_getvect( 0x9F );
  interrupt_func old_int_0xF0 = _dos_getvect( 0xF0 );
  
  // interrupt vector table
  far_ptr_t __far * ivt = 0;
  
  _disable();
  ivt[0x9F].offs = gfx_mode; // some sort of global variable :)
  
  // in function at file offset: 0x2B1B
  //   checked at file offset 0x2B23 in ega_vga.exe
  
  _enable();

  // new interrupt routines
#if SET_INT_24h  
  _dos_setvect( 0x24, (interrupt_func)interrupt_24h );
#endif  
  _dos_setvect( 0x97, (interrupt_func)interrupt_97h );
 
  // load sound TSR

  if( sound_type < 3 )
  {
    int sound_res = spawnl( P_WAIT, sound_exec[sound_type], NULL );
    printf("sound_res: %i\n", sound_res);
  }

  // start game 

  int game_res = spawnl( P_WAIT, game_exec[gfx_mode], NULL );
  printf("game_res: %i\n", game_res);

  // restore interrupts
  
  // sound driver active?
  if(old_int_0xF0 != _dos_getvect( 0xF0 ))
  {
    uint8_t far* int_f0 = (uint8_t far*)MK_FP(ivt[0xF0].segm, ivt[0xF0].offs);
    
    if( _fmemcmp(int_f0+2, "IFGM", 4) == 0 )
    {
      printf("uninstall sound TSR...\n");
      REGS inregs;
      inregs.h.ah = 0xE;
      int86(0xF0,&inregs,&inregs);   

      if(old_int_0xF0 == _dos_getvect( 0xF0 ))
      {
        printf("sound driver uninstalled!\n");
      }
      else
      {
        printf("sound driver uninstall failed!\n");
      }
    }
  }
  
#if SET_INT_24h  
  _dos_setvect( 0x24, old_int_0x24 );
#endif  
  _dos_setvect( 0x97, old_int_0x97 );
  _dos_setvect( 0x9F, old_int_0x9F );

  return 0;
}

