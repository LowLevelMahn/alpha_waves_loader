.model small
.stack 200h

ptr16 struc ; sizeof=0x4
  ofs dw ?
  segm dw ?
ptr16 ends

dseg segment 'DATA'
  game_exe db 'ega_vga.exe',0
  adlib_com db 's_adlib.com',0
  commandline db ' ',0dh
  
params:
  dw 0 ; copy caller environment if 0
  dw offset commandline
  dw seg commandline
  dw 0ffffh,0ffffh ; fcb1
  dw 0ffffh,0ffffh ; fcb2
  
  error_msg_tsr db 'exec tsr failed',0dh,0ah,'$'
  error_msg_game db 'exec game failed',0dh,0ah,'$'
dseg ends

cseg segment 'CODE'   
  assume cs:cseg
  
  feature_flags dw 0C001h
  dos_version db 5
  gfx_mode dw 4 ; 4=VGA
  
interrupt_97h:
  mov ax,cs:feature_flags
  iret

interrupt_24h:
  mov al, cs:dos_version
  iret

start:        
  mov ax,dseg
  mov ds,ax
  mov es,ax
  
  ; set interrupt 97h
  push ds
  mov ah,25h
  mov al,97h ; ivt[0x97]
  push cs
  pop ds
  mov dx,offset interrupt_97h
  int 21h
  pop ds
  
  ; or patch the exe itself
  ; at fileoffset 57DA in ae_vga.exe replace bytes {CD 97 8B D8 25 00 C0} with {bb 01 c0 b8 00 c0 90}
  ; gets you only EGA colors due to missing Interrupt[9Fh].offset overwrite with gfx_mode

  ; set interrupt 24h
  push ds
  mov ah,25h
  mov al,24h ; ivt[0x24]
  push cs
  pop ds
  mov dx,offset interrupt_24h
  int 21h
  pop ds

  ; overwrite Interrupt 9Fh offset with gfx_mode value
  push ds
  mov bx,cs:gfx_mode ; gfx_mode = VGA - or else the colors look like the EGA version
  xor ax, ax
  mov ds, ax
  assume ds:nothing
  mov ds:(9Fh*sizeof ptr16), bx ; ivt[0x9F]
  pop ds  

  ; ---- start adlib sound TSR
  
  mov dx,offset adlib_com
  mov bx,offset params
  mov ax,4B00h
  
  push ds ; save ds for return
  push es ; save es for return  
  ; seems to be needed for old dos versions
  mov cs:[stk_seg],ss ; save stack seg and
  mov cs:[stk_ptr],sp ; stack pointer in cseg  

  ; starts sound TSR
  int 21h
  jnc start_game
  
  ; ax = 8 = Insufficient memory
  mov dx,offset error_msg_tsr
  mov ah,09h
  int 21h
  
  jmp exit

start_game:

  mov dx,offset game_exe
  mov bx,offset params
  mov ax,4B00h
  
  push ds ; save ds for return
  push es ; save es for return  
  ; seems to be needed for old dos versions
  mov cs:[stk_seg],ss ; save stack seg and
  mov cs:[stk_ptr],sp ; stack pointer in cseg  

  ; starts game
  int 21h
  jnc execute_ok
  
  ; ax = 8 = Insufficient memory
  mov dx,offset error_msg_game
  mov ah,09h
  int 21h
  
  jmp exit
  
execute_ok:  
  cli
  mov ss,cs:[stk_seg]
  mov sp,cs:[stk_ptr]
  pop es
  pop ds
  sti

  ; shutdown adlib tsr
  xor ax,ax
  mov es,ax
  lds si, es:(0F0h*sizeof ptr16)
  mov ax, ds
  or  ax, si
  jz  short exit
  ; starts at offset 2 of interrupt F0 with 'IFGM ADLIB' if adlib tsr loaded
  mov cx, [si+2]
  mov dx, [si+4]
  cmp cx, 'FI'
  jnz short exit
  cmp dx, 'MG'
  jnz short exit
  mov ah, 0Eh
  int 0F0h    ; Int 0xF0 gets overwritten by ADLIB.COM TSR

exit:
  mov ax,4C00h
  int 21h

stk_ptr dw 0
stk_seg dw 0    

cseg ends

end start
