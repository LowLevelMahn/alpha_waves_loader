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

bool parse_cmd(int argc_, char* argv_[], uint16_t* gfx_mode_, uint16_t* sound_type_)
{
  if( argc_ < 3)
  {
    return false;
  }

  *gfx_mode_ = 0;
  static const char* gfx_str[5] = {"cga","ega","tandy","herc","vga"};
  bool found = false;
  for(int i = 0; i < 5; ++i)
  {
    if(strcmp(gfx_str[i], argv_[1]) == 0)
    {
      *gfx_mode_ = i;
      found = true;
      break;
    }
  }
  if(!found)
  {
    printf("unknown gfx: %s\n", argv_[1]);
    return false;
  }
  
  *sound_type_ = 0;
  static const char* sound_str[4] = {"adlib","tandy","pc","none"};
  found = false;
  for(int i = 0; i < 5; ++i)
  {
    if(strcmp(sound_str[i], argv_[2]) == 0)
    {
      *sound_type_ = i;
      found = true;
      break;
    }
  }
  if(!found)
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

const char* sound_exec[3] = {"s_adlib.com","s_tandy.com","s_pc_buz_com"};

const uint16_t feature_flags = 0xC001;
const uint8_t dos_version = 5;

void __interrupt __far interrupt_24h(INTPACK regs_)
{
  regs_.h.al = dos_version;
}

void __interrupt __far interrupt_97h(INTPACK regs_)
{
  regs_.x.ax = feature_flags;
}

typedef void __interrupt __far (*interrupt_func)();

int main(int argc_, char* argv_[])
{
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

  interrupt_func old_int_0x24 = _dos_getvect( 0x24 );  
  interrupt_func old_int_0x97 = _dos_getvect( 0x97 );
  interrupt_func old_int_0x9F = _dos_getvect( 0x9F );
  interrupt_func old_int_0xF0 = _dos_getvect( 0xF0 );
  
  // interrupt vector table
  far_ptr_t __far * ivt = 0;
  
  _disable();
  ivt[0x9F].offs = gfx_mode; // some sort of global variable :)
  _enable();

  // new interrupt routines
  _dos_setvect( 0x24, (interrupt_func)interrupt_24h );
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
      printf("uninstall sound TSR\n");
      REGS inregs;
      inregs.h.ah = 0xE;
      int86(0xF0,&inregs,&inregs);   
    }
    _dos_setvect( 0xF0, old_int_0xF0 );
  }
  
  _dos_setvect( 0x24, old_int_0x24 );
  _dos_setvect( 0x97, old_int_0x97 );
  _dos_setvect( 0x9F, old_int_0x9F );

  return 0;
}

