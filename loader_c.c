#include <i86.h> // for MK_FP etc.

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned long uint32_t;

#define TOKENPASTE(x, y) x ## y
#define TOKENPASTE2(x, y) TOKENPASTE(x, y)

#define STATIC_ASSERT(COND) struct TOKENPASTE2(ASSERT_STRUCT_, __LINE__){char static_assertion[(COND)?1:-1];};
#define OFFSETOF(type, field)((unsigned long) (&(((type*)0)->field)))

//STATIC_ASSERT(sizeof(int8_t)==1)
STATIC_ASSERT(sizeof(uint8_t)==1)
//STATIC_ASSERT(sizeof(int16_t)==2)
STATIC_ASSERT(sizeof(uint16_t)==2)
//STATIC_ASSERT(sizeof(int32_t)==4)
STATIC_ASSERT(sizeof(uint32_t)==4)

#pragma pack ( push, 1 )

struct far_ptr_t
{
  uint16_t segm;
  uint16_t offs;
};

#pragma pack(pop)

// keeps offset as small as possible
#define NORMALIZE_PTR(SEG_, OFS_) \
{ \
  SEG_ += (OFS_) / 16; \
  OFS_ = (OFS_) % 16; \
}

// TODO
// 1. inline asm with aux
// 2. replace with C code

#if 0
// C mangling with _ prefix
extern "C" 
{
  void __cdecl __near cc_GAME_START_sub_3(uint16_t es_,	uint16_t di_, uint16_t ds_, uint16_t si_);
  
  far_ptr_t another_pointer2;
  uint8_t byte_57;
  uint16_t word_60;
  uint8_t byte_569;
}
#else
extern struct far_ptr_t another_pointer2;
extern uint8_t byte_57;
extern uint16_t word_60;
extern uint8_t byte_569;
#endif

static struct far_ptr_t* get_ivt()
{
  return (struct far_ptr_t*)MK_FP(0,0);
}

const char my_string[] = "blub";

void __cdecl __near cc_GAME_START_sub_3(uint16_t es_,	uint16_t di_, uint16_t ds_, uint16_t si_)
{
#if 1
  inline void asm_cc_GAME_START_sub_3();
  #pragma aux asm_cc_GAME_START_sub_3 = \
/* set registers */ \
"    mov	es, es_" \
"    mov di, di_" \
"    mov	ds, ds_" \
"    mov si, si_" \
"sub_3_begin:    " \
"    push  es" \
"    push  di" \
"      mov cx, 128" \
"      mov ax, ds" \
"      mov es, ax" \
"      mov di, 301h" \
"      xor ax, ax" \
"      rep stosw" \
"    pop di" \
"    pop es" \
/* */ \
"    push  es" \
"      xor ax, ax" \
"      mov es, ax" \
"      sub di, es:4" \
"    pop es" \
/* */ \
/*   normalize ptr */ \
"    mov ax, di" \
"    shr ax, 1" \
"    shr ax, 1" \
"    shr ax, 1" \
"    shr ax, 1" \
"    mov cx, es" \
"    add cx, ax" \
"    mov es, cx" \
"    and di, 0Fh" \
/* */ \
"    push  es" \
"      xor ax, ax" \
"      mov es, ax" \
"      add di, es:4" \
"    pop es" \
/* */ \
"    push  ds" \
"    push  es" \
"    push  si" \
"    push  di" \
/* */ \
"      mov cx, 4" \
"      mov di, offset byte_57" \
"      mov ax, cs" \
"      mov es, ax" \
"      lds si, dword ptr cs:another_pointer2" \
/* */ \
/*     normalize ptr */ \
"      mov ax, si" \
"      shr ax, 1" \
"      shr ax, 1" \
"      shr ax, 1" \
"      shr ax, 1" \
"      mov dx, ds" \
"      add ax, dx" \
"      mov ds, ax" \
"      and si, 0Fh" \
/* */ \
"      mov word ptr cs:another_pointer2+0, si" \
"      mov word ptr cs:another_pointer2+2, ds" \
"      add word ptr cs:another_pointer2+0, cx" \
"      rep movsb" \
/* */ \
"    pop di" \
"    pop si" \
"    pop es" \
"    pop ds" \
/* */ \
"    mov dx, cs:word_60" \
"    inc dx" \
"    cmp cs:byte_57, 0" \
"    jnz short loc_565" \
"    jmp loc_566" \
/* */ \
"loc_565:" \
"    push  ds" \
"    push  es" \
"    push  di" \
"    xor ch, ch" \
"    mov cl, cs:byte_57" \
"    mov di, 201h" \
"    mov ax, ds" \
"    mov es, ax" \
"    mov ds, word ptr cs:another_pointer2+2" \
"    mov si, word ptr cs:another_pointer2+0" \
"    add word ptr cs:another_pointer2+0, cx" \
"    rep movsb" \
"    mov cl, cs:byte_57" \
"    xor ch, ch" \
"    mov di, 1" \
"    add word ptr cs:another_pointer2+0, cx" \
"    rep movsb" \
"    mov cl, cs:byte_57" \
"    mov di, 101h" \
"    add word ptr cs:another_pointer2+0, cx" \
"    rep movsb" \
"    pop di    " \
"    pop es    " \
"    pop ds    " \
"    xor ch, ch" \
"    mov cl, cs:byte_57" \
"    xor ah, ah" \
"    mov bx, 1" \
/* */ \
"loc_567:" \
"    mov al, [bx+200h]" \
"    mov si, ax    " \
"    mov dl, [si+301h]" \
"    mov [bx+402h], dl" \
"    mov [si+301h], bl" \
"    inc bx" \
"    loop  loc_567" \
"    mov dx, cs:word_60" \
"    inc dx" \
"    mov cx, 1" \
/* */ \
"loc_124:" \
"    dec dx" \
"    jnz short loc_568" \
/* */ \
"loc_577:" \
"    cmp cs:byte_569, 0" \
"    jz  short locret_570" \
"    jmp sub_3_begin" \
/* */ \
"locret_570:" \
"    jmp end" \
/* */ \
"loc_568:" \
"    push  ds" \
"    mov si, word ptr cs:another_pointer2+2" \
"    mov ds, si" \
"    mov si, word ptr cs:another_pointer2+0" \
"    lodsb" \
"    mov word ptr cs:another_pointer2+0, si" \
"    pop ds" \
"    mov bx, ax" \
"    cmp byte ptr [bx+301h], 0" \
"    jnz short loc_571" \
"    stosb" \
"    jmp short loc_124" \
/* */ \
"loc_571:" \
"    mov bl, [bx+301h]" \
"    xor ax, ax" \
"    push  ax" \
"    jmp short loc_128" \
/* */ \
"loc_129:" \
"    mov bp, ax" \
"    cmp byte ptr ds:[bp+301h], 0" \
"    jz  short loc_572" \
"    cmp bl, ds:[bp+301h]" \
"    ja  short loc_573" \
"    mov al, bl" \
"    mov bl, ds:[bp+301h]" \
/* */ \
"loc_575:" \
"    mov bl, [bx+402h]" \
"    or  bl, bl" \
"    jz  short loc_574" \
"    cmp bl, al" \
"    jb  short loc_128" \
"    jmp short loc_575" \
/* */ \
"loc_573:" \
"    mov bl, ds:[bp+301h]" \
/* */ \
"loc_128:" \
"    mov al, [bx+100h]" \
"    mov ah, bl" \
"    push  ax" \
"    xor ah, ah" \
"    mov al, [bx]" \
"    jmp short loc_129" \
/* */ \
"loc_574:" \
"    mov ax, bp" \
/* */ \
"loc_572:" \
"    stosb" \
"    pop ax" \
"    or  ax, ax" \
"    jnz short loc_576" \
"    jmp short loc_124" \
/* */ \
"loc_576:" \
"    mov bl, ah" \
"    xor ah, ah" \
"    jmp short loc_129" \
/* */ \
"loc_566:" \
"    push  ds" \
"    push  es" \
"    mov cx, cs:word_60" \
"    push  cx" \
"    mov ds,word ptr cs:another_pointer2+2" \
"    mov si,word ptr cs:another_pointer2+0" \
"    add word ptr cs:another_pointer2+0, cx" \
"    rep movsb" \
"    pop cx" \
"    pop es" \
"    pop ds" \
"    jmp loc_577" \
"    end:"
  asm_cc_GAME_START_sub_3();

#else
  uint16_t es = es_;
  uint16_t di = di_;
  uint16_t ds = ds_;
  uint16_t si = si_;
  
  uint8_t far* ds_301h = (uint8_t far *)MK_FP(ds_, 0x301);
  for(int i = 0; i < 512; ++i)
  {
    ds_301h[i] = 0;
  }

  far_ptr_t* ivt = get_ivt();

  di -= ivt[1].offs;
  NORMALIZE_PTR(es,di);
  di += ivt[1].offs;  
#endif
}

