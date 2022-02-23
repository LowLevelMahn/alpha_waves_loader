#ifdef WITH_MAIN
#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#endif

#if defined(__DMC__)
#include <dos.h>
#else
#include <i86.h>
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

#define VERSION 1

#if VERSION == 0

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
modify [ax dx si di cx bx];

  inline_asm_memcopy(es_, di_, ds_, si_, bx_, cx_);
}

#endif

// keeps offset as small as possible
#define NORMALIZE_PTR(SEG_, OFS_) \
  { \
    SEG_ += (OFS_ / 16); \
    OFS_ = OFS_ % 16; \
  }
  
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
  
#if VERSION == 1

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
    
#if 0
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

// tests
void main()
{
  uint32_t size = 0;
  uint8_t far* buffer1 = 0;
  uint8_t far* buffer2 = 0;
  uint16_t hi_size = 0;
  uint16_t lo_size = 0;
  uint32_t i = 0;
  
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
