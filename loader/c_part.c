#ifdef WITH_MAIN
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#endif

#if defined(__DMC__) || defined(__BORLANDC__)
#include <dos.h>
#else
#include <i86.h>
#endif

#if defined(__BORLANDC__)  
// BCC 5.02 got no inline/__inline/__forceinline ?
#define inline
#endif

typedef signed char int8_t;
typedef unsigned char uint8_t;
typedef signed short int16_t;
typedef unsigned short uint16_t;
typedef signed long int32_t;
typedef unsigned long uint32_t;

#define TOKENPASTE(x, y) x ## y
#define TOKENPASTE2(x, y) TOKENPASTE(x, y)

#define STATIC_ASSERT(COND) struct TOKENPASTE2(ASSERT_STRUCT_, __LINE__){char static_assertion[(COND)?1:-1];};
#define OFFSETOF(type, field)((unsigned long) (&(((type*)0)->field)))

STATIC_ASSERT(sizeof(int8_t)==1)
STATIC_ASSERT(sizeof(uint8_t)==1)
STATIC_ASSERT(sizeof(int16_t)==2)
STATIC_ASSERT(sizeof(uint16_t)==2)
STATIC_ASSERT(sizeof(int32_t)==4)
STATIC_ASSERT(sizeof(uint32_t)==4)

#pragma pack ( push, 1 )

struct far_ptr_t
{
  uint16_t segm;
  uint16_t offs;
};

#pragma pack(pop)

//TODO:
//read_config_and_resize_memory
//START_GAME_FEATURE_FLAG_STUFF_sub_21
//SIMPLE_INIT_routine
//GAME_START_sub_5
//GAME_START_sub_6
//GAME_START_sub_7

#define MEMCOPY_VERSION 1

#if MEMCOPY_VERSION == 0

// only compiles with Watcom wcc

// original reverse engineering function
void cdecl near memcopy_c(uint16_t es_, uint16_t di_, uint16_t ds_, uint16_t si_, uint16_t bx_, uint16_t cx_)
{
static inline void inline_asm_memcopy(uint16_t es_, uint16_t di_, uint16_t ds_, uint16_t si_, uint16_t bx_, uint16_t cx_);
#pragma aux inline_asm_memcopy = \
  /* save regs */ \
  "  push  ds" \
  "  push  es" \
  "  push  si" \
  "  push  di" \
  "  push  bx" \
  "  push  cx" \
  /* set segs */ \
  "  mov es,ax " \
  "  mov ds,dx " \
  "  cld" \
  "copy_block:" \
  /* normalize_ptr(ds,si) */ \
    "  mov ax, si" \
    "  shr ax, 1" \
    "  shr ax, 1" \
    "  shr ax, 1" \
    "  shr ax, 1" \
    "  mov dx, ds" \
    "  add dx, ax" \
    "  mov ds, dx" \
    "  and si, 0Fh" \
  /* normalize_ptr(es,di) */ \
    "  mov ax, di" \
    "  shr ax, 1" \
    "  shr ax, 1" \
    "  shr ax, 1" \
    "  shr ax, 1" \
    "  mov dx, es" \
    "  add dx, ax" \
    "  mov es, dx" \
    "  and di, 0Fh" \
  /* ax and dx values not needed */ \    
  /* calc block size */ \
    "  mov ax, 512" \
    /* underflow? */ \
    "  sub cx, ax" \
    "  sbb bx, 0" \
    /* --- */ \
    "  jnb short loc_555" \
    "  add ax, cx" \
    "  mov bx,0" \
    "  mov cx,0" \
    "loc_555:" \
  /* copy block */ \
    "  push cx" \
    "  mov cx, ax" \
    "  rep movsb" \
    "  pop cx" \
    "  mov ax, cx" \
/* all bytes copied? */ \
  "  or  ax, bx" \
  "  jnz short copy_block" \
/* restore regs */ \  
  "  pop  cx" \
  "  pop  bx" \
  "  pop  di" \
  "  pop  si" \
  "  pop  es" \
  "  pop  ds" \
parm[ax][di][dx][si][bx][cx] \
__modify [__ax __dx __si __di __cx __bx];

  inline_asm_memcopy(es_, di_, ds_, si_, bx_, cx_);
}

#endif

// keeps offset as small as possible
#define NORMALIZE_PTR(SEG_, OFS_) \
  { \
    SEG_ += (OFS_ / 16); \
    OFS_ = OFS_ % 16; \
  }
 
// does not crash game if inlined
static /*inline*/ void normalize_ptr(uint16_t* seg_, uint16_t* ofs_)
{
  NORMALIZE_PTR(*seg_, *ofs_);
}

static uint32_t inline hi_lo(uint16_t hi_, uint16_t lo_)
{
  uint32_t value = hi_;
  value = ( value << 16 ) + lo_;
  return value;
}
  
#if MEMCOPY_VERSION == 1

// C port
void cdecl near memcopy_c(uint16_t es_, uint16_t di_, uint16_t ds_, uint16_t si_, uint16_t bx_, uint16_t cx_)
{
  uint32_t rest = 0;
  uint32_t bytes = 0;
  uint16_t i = 0;
  uint8_t far* dest = 0;
  uint8_t far* src = 0;
  uint32_t size = hi_lo(bx_, cx_); 
  
  rest = size;
  while(rest > 0)
  {
    bytes = rest < 512 ? rest : 512;
    rest -= bytes;
    
#if 0 // crash at call cs:0A6A
    // crashes game (tested with wcc/dmc/bcc + wlink/ulink)
    normalize_ptr(&es_, &di_);
    normalize_ptr(&ds_, &si_);
#else
    NORMALIZE_PTR(es_,di_);
    NORMALIZE_PTR(ds_,si_);
#endif    
    dest = (uint8_t far*)MK_FP(es_, di_);
    src = (uint8_t far*)MK_FP(ds_, si_);
    for(i = 0; i < bytes; ++i)
    {
      dest[i] = src[i];
      ++di_;
      ++si_;
    }
  }
}

#endif

#define CODE_SEG_VAR __based( __segname("_CODE") )

#define read_config_and_resize_memory_VERSION 0

#if read_config_and_resize_memory_VERSION == 0

#ifdef WITH_MAIN

char* config_tat_filename = "config.tat";
uint8_t config_tat_buffer[563];
uint16_t config_tat_size;
struct far_ptr_t maybe_exe_buffer;
uint16_t some_feature_flags;
uint8_t* config_tat_game_name_string;
uint8_t* config_tat_publisher_string;
uint8_t* config_tat_disk_name_string;
uint8_t* config_tat_content_end;
uint8_t* config_tat_gfx_table_offset;

#else

extern char* CODE_SEG_VAR config_tat_filename;
extern uint8_t* CODE_SEG_VAR config_tat_buffer;
extern uint16_t CODE_SEG_VAR config_tat_size;
extern struct far_ptr_t CODE_SEG_VAR maybe_exe_buffer;
extern uint16_t CODE_SEG_VAR some_feature_flags;
extern uint8_t* CODE_SEG_VAR config_tat_game_name_string;
extern uint8_t* CODE_SEG_VAR config_tat_publisher_string;
extern uint8_t* CODE_SEG_VAR config_tat_disk_name_string;
extern uint8_t* CODE_SEG_VAR config_tat_content_end;
extern uint8_t* CODE_SEG_VAR config_tat_gfx_table_offset;

#endif

static inline void dos_print(const char* dos_string_)
{
  static inline void asm_dos_print(uint16_t seg_, uint16_t ofs_);
  #pragma aux asm_dos_print = \
    "  mov ds,ax" \
    "  mov ah,09h" \
    "  int 21h" \
  parm[ax][dx]

  asm_dos_print(FP_SEG(dos_string_), FP_OFF(dos_string_));
}

static inline void dos_exit(uint8_t exit_code_)
{
  static inline void asm_dos_exit(uint8_t error_code_);
  #pragma aux asm_dos_exit = \
    "  mov ah,4ch" \
    "  int 21h" \
  parm[al]
    
  asm_dos_exit(exit_code_);
}

static inline uint16_t dos_file_open(const char* filename_, uint8_t open_mode_, uint16_t* handle_ )
{
  uint16_t fname_seg = FP_SEG(filename_);
  uint16_t fname_ofs = FP_OFF(filename_);
  uint16_t handle = 0;
  uint16_t result = 0;
  
  static inline void asm_dos_file_open();
  #pragma aux asm_dos_file_open = \
    " mov ax,fname_seg " \
    " mov ds, ax " \
		" mov	dx, fname_ofs " \
    " mov ah, 3Dh " \
    " mov al, open_mode_" \
    " int 21h " \
    " jc error " \
    " mov handle, ax " \
    " jmp end " \
    "error: " \
    "  mov result,ax " \
    "end: "
    
  asm_dos_file_open();
  
  *handle_ = handle;

  return result;  
}

static inline uint16_t dos_file_read(uint16_t handle_, uint8_t* buffer_, uint16_t bytes_to_read_, uint16_t* bytes_read_ )
{
  uint16_t result = 0;
  uint16_t bytes_read = 0;
  uint16_t buffer_seg = FP_SEG(buffer_);
  uint16_t buffer_ofs = FP_OFF(buffer_);
  
  static inline void asm_dos_file_read();
  #pragma aux asm_dos_file_read = \
    " mov bx, handle_ " \
    " mov cx, bytes_to_read_ " \
    " mov ax, buffer_seg " \
    " mov dx, buffer_ofs " \
    " mov ds, ax " \
    " mov ah, 3Fh " \
    " int 21h " \
    " jc error " \
    " mov bytes_read, ax " \
    " jmp end " \
    "error: " \
    "  mov result,ax " \
    "end: " \
  __modify [__ax __bx __cx __dx]
  asm_dos_file_read();

  *bytes_read_ = bytes_read;

  return result;  
}

static inline uint16_t dos_file_close(uint16_t handle_)
{
  uint16_t result = 0;

  static inline void asm_dos_file_close();
  #pragma aux asm_dos_file_close = \
    " mov bx, handle_ " \
    " mov ah, 3Eh " \
    " int 21h " \
    " jc error " \
    "  mov result,0 " \
    " jmp end " \
    "error: " \
    "  mov result,ax " \
    "end: " \
  __modify [__ax __bx]
  asm_dos_file_close();

  return result;  
}

static inline uint16_t dos_resize_memory(uint16_t seg_, uint16_t new_size_in_paragraphs, uint16_t* largest_available_block_)
{
  uint16_t result = 0; // 7 = corrupted MCB, 8 = not enough free memory, 9 = segment not found in MCB
  uint16_t largest_available_block = 0;

  static inline void asm_dos_resize_memory();
  #pragma aux asm_dos_resize_memory = \
    " mov ax, seg_ "  \
    " mov bx, new_size_in_paragraphs " \
    " mov dx, ax " \
    " mov ah, 4Ah " \
    " int 21h " \
    " jc error " \
    "  mov result,0 " \
    " jmp end " \
    "error: " \
    "  cmp ax,8 " \
    "  jne error_code " \
    "  mov largest_available_block,bx " \
    "error_code: " \
    "  mov result,ax " \
    "end: " \
  __modify [__ax __bx]
  asm_dos_resize_memory();
  
  *largest_available_block_ = largest_available_block;
  
  return result;  
}

static inline uint16_t dos_alloc_memory(uint16_t* seg_, uint16_t paragraphs_, uint16_t* largest_available_block_)
{
  uint16_t result = 0; // 7 = corrupted MCB, 8 = not enough free memory, 9 = segment not found in MCB
  uint16_t segm = 0;
  uint16_t largest_available_block = 0;

  static inline void asm_dos_alloc_memory();
  #pragma aux asm_dos_alloc_memory = \
    " mov bx, paragraphs_ " \
    " mov ah, 48h " \
    " int 21h " \
    " jc error " \
    " mov result,0 " \
    " mov segm,ax " \
    " jmp end " \
    "error: " \
    "  cmp ax,8 " \
    "  jne error_code " \
    "  mov largest_available_block,bx " \
    "error_code: " \
    "  mov result,ax " \
    "end: " \
  __modify [__ax __bx]
  asm_dos_alloc_memory();
  
  *seg_ = segm;
  *largest_available_block_ = largest_available_block;
  
  return result;  
}

#ifdef WITH_MAIN
void DumpHex(const void* data, size_t size) {
	char ascii[17];
	size_t i, j;
	ascii[16] = '\0';
	for (i = 0; i < size; ++i) {
		printf("%02X ", ((unsigned char*)data)[i]);
		if (((unsigned char*)data)[i] >= ' ' && ((unsigned char*)data)[i] <= '~') {
			ascii[i % 16] = ((unsigned char*)data)[i];
		} else {
			ascii[i % 16] = '.';
		}
		if ((i+1) % 8 == 0 || i+1 == size) {
			printf(" ");
			if ((i+1) % 16 == 0) {
				printf("|  %s \n", ascii);
			} else if (i+1 == size) {
				ascii[(i+1) % 16] = '\0';
				if ((i+1) % 16 <= 8) {
					printf(" ");
				}
				for (j = (i+1) % 16; j < 16; ++j) {
					printf("   ");
				}
				printf("|  %s \n", ascii);
			}
		}
	}
}
#endif

void cdecl near read_config_and_resize_memory_C()
{
  uint16_t file_handle = 0;
  uint16_t result = 0;
  uint16_t bytes_read = 0;
  uint16_t i = 0;
  
  uint16_t free_paragraphs = 0;
  uint8_t size_type = 0; // 0,1,2,3
  uint16_t adjust_paragraphs = 0;
  uint16_t available_paragraphs = 0;
  uint16_t allocated_paragraphs = 0;

  uint16_t file_offset0 = 0;
  uint16_t file_offset1 = 0;
  uint16_t file_offset2 = 0;
  uint16_t file_offset3 = 0;
  uint16_t file_offset4 = 0;

  //i can't use global vars or const strings here - they are wrongly aligned in the obj-file

  //dos_print("opening config.tat\r\n$");
  result = dos_file_open(config_tat_filename, 0, &file_handle);
  if( result != 0)
  {
    dos_print("error opening config.tat\r\n$");
#ifdef WITH_MAIN
    printf("--> result: %u\n", result);
#endif    
    dos_exit(1);
  }

  //dos_print("reading config.tat\r\n$");
  result = dos_file_read(file_handle, config_tat_buffer, 65535, &bytes_read);
  if( result != 0)
  {
    dos_print("error reading config.tat\r\n$");
#ifdef WITH_MAIN
    printf("--> result: %u\n", result);
#endif    
    dos_exit(1);
  }

#ifdef WITH_MAIN
  printf("bytes read: %u\n", bytes_read);

  DumpHex(config_tat_buffer, bytes_read);
#endif

  //dos_print("process config.tat\r\n$");
  config_tat_size = bytes_read;
  maybe_exe_buffer.offs = FP_OFF(config_tat_buffer) + config_tat_size;
  maybe_exe_buffer.segm = FP_SEG(config_tat_buffer);
  
  //dos_print("closing config.tat\r\n$");
  result = dos_file_close(file_handle);
  if( result != 0)
  {
    dos_print("error closing config.tat\r\n$");
#ifdef WITH_MAIN
    printf("--> result: %u\n", result);
#endif    
    dos_exit(1);
  }
  
  file_offset0 = *(uint16_t*)&config_tat_buffer[0];
  file_offset1 = *(uint16_t*)&config_tat_buffer[2];
  file_offset2 = *(uint16_t*)&config_tat_buffer[4];
  file_offset3 = *(uint16_t*)&config_tat_buffer[6];
  file_offset4 = *(uint16_t*)&config_tat_buffer[8];
  
#if WITH_MAIN  
  printf("config_tat_buffer[0] = 0x%04X\n", file_offset0);
  printf("config_tat_buffer[2] = 0x%04X\n", file_offset1);
  printf("config_tat_buffer[4] = 0x%04X\n", file_offset2);
  printf("config_tat_buffer[6] = 0x%04X\n", file_offset3);
  printf("config_tat_buffer[8] = 0x%04X\n", file_offset4);
#endif  
  
  //dos_print("set offsets\r\n$");
  config_tat_gfx_table_offset = config_tat_buffer + file_offset0;
  config_tat_game_name_string = config_tat_buffer + file_offset1;
  config_tat_publisher_string = config_tat_buffer + file_offset2;
  config_tat_disk_name_string = config_tat_buffer + file_offset3;
  config_tat_content_end = config_tat_buffer + file_offset4;

#if WITH_MAIN  
  // there are control-chars in the strings - that is correct
  printf("config_tat_game_name_string: %s\n", config_tat_game_name_string);  
  printf("config_tat_publisher_string: %s\n", config_tat_publisher_string);
  printf("config_tat_disk_name_string: %s\n", config_tat_disk_name_string);
#endif  

  // https://stanislavs.org/helppc/dos_error_codes.html
  // 07  Memory control blocks destroyed
	// 08  Insufficient memory
	// 09  Invalid memory block address
  
  //dos_print("resize maybe_exe_buffer\r\n$");
  adjust_paragraphs = (maybe_exe_buffer.offs / 16) + 1;
  result = dos_resize_memory(maybe_exe_buffer.segm, adjust_paragraphs, &available_paragraphs);
  if( result != 0)
  {
    dos_print("error resize maybe_exe_buffer\r\n$");
#ifdef WITH_MAIN
    printf("--> result: %u\n", result);
#endif
    dos_exit(1);
  }
  
#ifdef WITH_MAIN
  printf("available_paragraphs: %u\n", available_paragraphs);
#endif  

  //dos_print("alloc maybe_exe_buffer\r\n$");
  result = dos_alloc_memory(&maybe_exe_buffer.segm, 0xFFFF, &allocated_paragraphs);
  if( result != 0)
  {
    dos_print("error alloc maybe_exe_buffer\r\n$");
#ifdef WITH_MAIN
    printf("--> result: %u\n", result);
#endif    
    dos_exit(1);
  }

#ifdef WITH_MAIN
  printf("allocated_paragraphs: %u\n", allocated_paragraphs);
#endif  

  // 0b0011000000000001
	//   clear bits 1-11 and 14+15
	//   keep 1 and 12+13
	some_feature_flags &=	0x3001;
	size_type =
	  (allocated_paragraphs	>= 0x8000) ? 3
	  : (allocated_paragraphs >= 0x6000) ? 2
	  : (allocated_paragraphs >= 0x4000) ? 1
	  : 0;
	// set size_type in 14+15
	// set first bit
	some_feature_flags |=	((size_type << 14) + 1);
}

#endif

#ifdef WITH_MAIN

void test()
{
  uint16_t seg1 = 0x100;
  uint16_t ofs1 = 0x10;

  normalize_ptr(&seg1, &ofs1);
  printf("normalize_ptr: %x:%x\n", seg1, ofs1);

  seg1 = 0x100;
  ofs1 = 0x10;

  NORMALIZE_PTR(seg1, ofs1);
  printf("NORMALIZE_PTR: %x:%x\n", seg1, ofs1);
}

static const char* dos_error_code_str(uint16_t error_code_)
{
  switch(error_code_)
  {
    case 0x01: return "Invalid function number";
    case 0x02: return "File not found";
    case 0x03: return "Path not found";
    case 0x04: return "Too many open files (no handles left)";
    case 0x05: return "Access denied";
    case 0x06: return "Invalid handle";
    case 0x07: return "Memory control blocks destroyed";
    case 0x08: return "Insufficient memory";
    case 0x09: return "Invalid memory block address";
    case 0x0A: return "Invalid environment";
    case 0x0B: return "Invalid format";
    case 0x0C: return "Invalid access mode (open mode is invalid)";
    case 0x0D: return "Invalid data";
    case 0x0E: return "Reserved";
    case 0x0F: return "Invalid drive specified";
    case 0x10: return "Attempt to remove current directory";
    case 0x11: return "Not same device";
    case 0x12: return "No more files";  
  }
  return "unknown error";
}

// tests
void main()
{
  uint32_t size = 0;
  uint8_t far* buffer1 = 0;
  uint8_t far* buffer2 = 0;
  uint16_t hi_size = 0;
  uint16_t lo_size = 0;
  uint32_t i = 0;
 
  read_config_and_resize_memory_C();
  return;
 
  test();
  
  size = 60000;
  buffer1 = _fmalloc(size);
  if( !buffer1 )
  {
      printf("can't alloc buffer1!\n");
      exit(1);    
  }
  buffer2 = _fmalloc(size);
  if( !buffer2 )
  {
      printf("can't alloc buffer2!\n");
      exit(1);    
  }
  
  for(i = 0; i < size; ++i)
  {
    buffer1[i] = 0;
    buffer2[i] = (uint8_t)i;
  }

  for(i = 0; i < size; ++i)
  {
    if( buffer1[i] != 0 )
    {
      printf("buffer1[%u] != 0\n");
      exit(1);
    }
    if( buffer2[i] != (uint8_t)i )
    {
      printf("buffer2[%u] != %X\n", (uint8_t)i);
      exit(1);
    }
  }

  hi_size = size >> 16;
  lo_size = size & 0xFFFF;

  printf("hi: %u, lo: %u\n", hi_size, lo_size);

#if 1
#if 0
  _fmemcpy(buffer1, buffer2, size);
#endif  
  memcopy_c(FP_SEG(buffer1), FP_OFF(buffer1), FP_SEG(buffer2), FP_OFF(buffer2), hi_size, lo_size);

  for(i = 0; i < size; ++i)
  {
    if( buffer1[i] != buffer2[i])
    {
      printf("buffer1[%u] != buffer2[%u]\n", i, i);
      exit(1);
    }
  }
#endif
  
  _ffree(buffer1);
  buffer1 = 0;
  _ffree(buffer2);
  buffer2 = 0;
}

#endif
