#include "emu.h>

/*
// int __cdecl code_question_two_sub_1288E(char far *code_answer_)
code_question_two_sub_1288E proc near   ; CODE XREF: code_question_screen_sub_17709+188p

code_answer_    = dword ptr  4

  push    bp              ; it could be that the code are some sort of segment or offsets - so the right one is needed to get a fully working game
  mov     bp, sp
  push    es
  push    ds
  push    si
  push    di
  les     si, [bp+code_answer_] ; input in code-question dialog:
                          ;
                          ; 4321 gives here bytes: 1F 1E 1D 1C
                          ; 0000 gives here bytes: 1B 1B 1B 1B
                          ; 9999 gives here bytes: 24 24 24 24
                          ;
                          ; 0-9 in question-dialog => 1B-24 here
  mov     di, offset maybe_table_word_254E6 ; 80 words

loc_1289B:                              ; DATA XREF: code_question_screen_sub_1283E+1Dw
  mov     ax, 1234h       ; !!! code modified by code_question_screen_sub_1283E
                          ;
                          ; always between: 1..10
                          ;
                          ; mov ax,1..10
  shl     ax, 1
  shl     ax, 1
  shl     ax, 1
  shl     ax, 1

loc_128A6:                              ; DATA XREF: code_question_screen_sub_12865+1Dw
  mov     bx, 1234h       ; !!! code modified by code_question_screen_sub_12865
                          ;
                          ; always: 0...8+28
                          ;
                          ; mov bx,0...8+28
  shl     bx, 1
  add     ax, bx
  add     di, ax
  xor     dx, dx
  mov     cx, [di]
  mov     di, 2B43h       ; constant
  mov     al, es:[si]     ; code_answer_[0]
  sub     al, 1Bh         ; transform to 0-9
  mov     ah, al
  shl     ah, 1
  shl     ah, 1
  add     ah, al
  add     ah, ah
  mov     al, es:[si+1]   ; code_answer_[1]
  sub     al, 1Bh         ; transform to 0-9
  add     ah, al
  mov     bx, 2AD2h       ; constant
  inc     dx
  cmp     ah, cl
  jnz     short loc_128E4
  mov     di, (offset loc_124D3+2) ; also used for code modification?
  shl     ax, 1
  shl     ah, 1
  inc     dx
  add     ah, al
  add     ax, di
  add     si, 4           ; at the end

loc_128E4:                              ; CODE XREF: code_question_two_sub_1288E+45j
  mov     al, es:[si-2]   ; code_answer_[2]
  sub     al, 1Bh         ; transform to 0-9
  mov     ah, al
  shl     ah, 1
  shl     ah, 1
  add     ah, al
  add     ah, ah
  mov     al, es:[si-1]   ; code_answer_[3]
  sub     al, 1Bh         ; transaform to 0-9
  add     ah, al
  inc     dx
  mov     bx, 2AD2h       ; constant, bx not in use, or used by caller?
  cmp     ah, ch
  jnz     short loc_12910
  mov     bx, (offset loc_121B0+1) ; modified place - here only read
  mov     ax, cs:[di]     ; prepare for modification - only read here
  shr     ax, 1
  inc     dx
  shl     word ptr cs:[bx], 1 ; modifies code at always fix point cs:21B1

loc_12910:                              ; CODE XREF: code_question_two_sub_1288E+74j
  add     cs:[di], ax     ; modifies code around
  mov     ax, dx          ; code question result: dx = 2 = wrong, dx = 4 = correct
  pop     di
  pop     si
  pop     ds
  pop     es
  pop     bp
  retn
code_question_two_sub_1288E endp
*/

void protection_tests()
{
}
