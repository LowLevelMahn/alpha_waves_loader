LONG_DIV_MUL = 1

; ---------------------------------------------------------------------------

ptr16		struc ;	(sizeof=0x4)	; XREF:	seg000:maybe_exe_bufferr
					; seg000:some_game_ptrr ...
ofs		dw ?
segm		dw ?
ptr16   ends

; ---------------------------------------------------------------------------

gfx_block_t	struc ;	(sizeof=0x18)
filename	db 18 dup(?)		; XREF:	start_0+30Ao start_0+324o ...
byte_12h	db ?			; XREF:	read_some_file_sub_4+54r
byte_13h	db ?			; XREF:	GAME_START_sub_7+1Dr
					; start_0+2FBr	...
byte_14h	db ?			; XREF:	START_GAME_IS_GFX_SUPPORTED_sub_12+5r
byte_15h	db ?
word_16h	dw ?			; XREF:	START_GAME_FEATURE_FLAG_STUFF_sub_21r
gfx_block_t	ends
; ---------------------------------------------------------------------------
CommandTail	struc ;	(sizeof=0x80)	; XREF:	sPSPr
count		db ?
buffer		db 127 dup(?)
CommandTail	ends
; ---------------------------------------------------------------------------
; Program segment prefix from dosbox source
sPSP		struc ;	(sizeof=0x100)
exit_code		db 2 dup(?)		; XREF:	EXE_HEADER_sub_2+140o
					; GAME_START_sub_6+13o
next_seg	dw ?
fill_1		db ?
far_call	db ?
cpm_entry	ptr16 <?>
int_22		ptr16 <?>
int_23		ptr16 <?>
int_24		ptr16 <?>
psp_parent	dw ?
files		db 20 dup(?)
environment	dw ?
stack		dd ?
max_files	dw ?
file_table	ptr16 <?>
prev_psp	ptr16 <?>
interim_flag	db ?
truename_flag	db ?
nn_flags	dw ?
version_dos	dw ?
fill_2		db 14 dup(?)
service		db 3 dup(?)
fill_3		db 9 dup(?)
fcb1		db 16 dup(?)
fcb2		db 16 dup(?)
fill_4		db 4 dup(?)
cmdtail_or_dta	CommandTail <?>		; XREF:	GAME_START_sub_6+2Eo ;	or DTA
sPSP		ends
; ---------------------------------------------------------------------------
; exe header from dosbox source
EXE_Header	struc ;	(sizeof=0x1C)
signature	dw ?			; XREF:	EXE_HEADER_sub_2:loc_560r
extrabytes	dw ?
pages		dw ?
relocations	dw ?
headersize	dw ?			; XREF:	EXE_HEADER_sub_2:loc_562r
minmemory	dw ?
maxmemory	dw ?
initSS		dw ?			; XREF:	EXE_HEADER_sub_2+E7r
initSP		dw ?			; XREF:	EXE_HEADER_sub_2+DFr
checksum	dw ?
initIP		dw ?			; XREF:	EXE_HEADER_sub_2+FBr
initCS		dw ?			; XREF:	EXE_HEADER_sub_2+F1r
reloctable	dw ?
overlay		dw ?
EXE_Header	ends

    .286
    .model tiny

; ===========================================================================

; Segment type: Pure code
seg000    segment byte public 'CODE'
    assume cs:seg000
    org 100h
    assume es:nothing, ss:nothing, ds:seg000

; =============== S U B R O U T I N E =======================================

; Attributes: noreturn thunk

    public start
start   proc near
    jmp start_0
start   endp

; ---------------------------------------------------------------------------

; Data

someway_cs_registe_value1 dw 0		; DATA XREF: GAME_START_sub_7+79w
					; GAME_START_sub_7+8Do
    db 80h, 0
someway_cs_registe_value2 dw 0		; DATA XREF: GAME_START_sub_7+7Ew
    db 5Ch, 0
someway_cs_registe_value3 dw 0		; DATA XREF: GAME_START_sub_7+83w
    db 6Ch, 0
someway_cs_registe_value4 dw 0		; DATA XREF: GAME_START_sub_7+88w
maybe_exe_buffer ptr16 <0>		; DATA XREF: read_some_file_sub_4+20r
          ; read_some_file_sub_4+3Er ...
					;
					;
					;
					; --------------
					;
					; the_overwrite_buffer_begin
					; [config.tat]
					; [maybe_exe_buffer]
					;
					;
					;
					; --------------
some_game_ptr	ptr16 <0>		; DATA XREF: read_some_file_sub_4+D7w
          ; read_some_file_sub_4+1B5r ...
word_44   dw 0      ; DATA XREF: read_some_file_sub_4+25w
          ; read_some_file_sub_4+C2w ...
word_45   dw 0      ; DATA XREF: read_some_file_sub_4+2Cw
          ; read_some_file_sub_4+C7w ...
far_ptr3	ptr16 <0>		; DATA XREF: GAME_START_sub_5+4Br
					; GAME_START_sub_6+3w ...
another_far_ptr	ptr16 <0>		; DATA XREF: read_some_file_sub_4:loc_580w
          ; read_some_file_sub_4+145r ...
somway_exe_buffer_seg dw 0		; DATA XREF: GAME_START_sub_5+Dr
					; GAME_START_sub_5+63r	...
some_register_ss_value dw 0		; DATA XREF: GAME_START_sub_7+96w
					; GAME_START_sub_7+A9r
some_register_sp_value dw 0		; DATA XREF: GAME_START_sub_7+91w
					; GAME_START_sub_7+A4r

maybe_10_ptr	ptr16 0Ah dup(<0>)	; DATA XREF: GAME_START_sub_6+5Do

					; start_0+35o ...
					; ----
					;
					; https://wiki.osdev.org/MZ is sizeof 28h
					;
					;
					; seems	do be always 0???
					;
					;
					; ----
also_a_pointer	ptr16 <0>		; DATA XREF: read_some_file_sub_4+137w
          ; read_some_file_sub_4:loc_584w ...
          
byte_55		db 0			; DATA XREF: EXE_HEADER_sub_2+1r
					; EXE_HEADER_sub_2+5Cr	...
          ; gets tested with test byte_55 and the following values
          ;   0xC0 = 0b11000000
          ;   0x40 = 0b01000000
          ;   0x20 = 0b00100000
          ;   0x18 = 0b00011000
          ;   0x10 = 0b00010000  
        
public C byte_57
byte_57		db 0			; DATA XREF: GAME_START_sub_3+42o
					; GAME_START_sub_3+7Cr	...
public C byte_569
byte_569	db 0			; DATA XREF: GAME_START_sub_3:loc_577r
public C word_60
word_60		dw 0			; DATA XREF: GAME_START_sub_3+76r
					; GAME_START_sub_3+EDr	...
    db 4 dup(0)
public C another_pointer2
another_pointer2 ptr16 <0>		; DATA XREF: GAME_START_sub_3+49r
					; GAME_START_sub_3+61w	...
word_62		dw 0			; DATA XREF: EXE_HEADER_sub_2+17w
					; EXE_HEADER_sub_2+9Ar	...
some_game_pointer_seg dw 0		; DATA XREF: EXE_HEADER_sub_2+1Cw
					; EXE_HEADER_sub_2+95r	...
new_psp_seg	dw 0			; DATA XREF: EXE_HEADER_sub_2+2Dr
					; EXE_HEADER_sub_2+103r ...
word_558	dw 0			; DATA XREF: EXE_HEADER_sub_2:loc_557w
some_register_cs_value dw 0		; DATA XREF: EXE_HEADER_sub_2+12w
register_sp_value dw 0			; DATA XREF: EXE_HEADER_sub_2+E3w
					; EXE_HEADER_sub_2+10Br
register_ss_value dw 0			; DATA XREF: EXE_HEADER_sub_2+EDw
					; EXE_HEADER_sub_2+10Fr
exe_cs_ip_ptr	ptr16 <0>		; DATA XREF: EXE_HEADER_sub_2+FFw
					; EXE_HEADER_sub_2+129r ...
; __int16 exe_pointer
exe_pointer	ptr16 <0>		; DATA XREF: EXE_HEADER_sub_2+21w
					; EXE_HEADER_sub_2+57r	...
pointer3	ptr16 <0>		; DATA XREF: EXE_HEADER_sub_2+4Dw
					; EXE_HEADER_sub_2+85r	...
start_psp dw 0      ; DATA XREF: start_0+11w start_0+8Fr
saved_int1_ptr	ptr16 <0>		; DATA XREF: EXE_HEADER_sub_2+3Br
					; start_0+9Er ...
saved_5_interrupt_pointers ptr16 5 dup(<0>) ; DATA XREF: start_0+B9o
          ; INIT_PART_init_stuff_sub_26+4Ao
    
config_tat_gfx_table_offset dw 0	; DATA XREF: GFX_SELECT_MENU_sub_9+27r
          ; read_config_and_resize_memory+4Aw ...
					; -------------------------------------------
					;
					;
					;
					; its a	pointer	to a uint16_t[5] table of offsets
          ;
          ; uint16_t* gfx_related_table
					;
					;
					; contains (i think) the code offsets for each grafic card type	in the progs.cc
					;
					;
					;
					;
					;
					; -------------------------------------------

config_tat_filename db 'Config.tat',0   ; DATA XREF: read_config_and_resize_memory+5o
          
some_feature_flags dw 1			; DATA XREF: read_config_and_resize_memory+BCr
					; read_config_and_resize_memory+C5w ...
					;
					;
					;
					; --------------------------
					; bit[ 0] = ??? [active on startup]
					; bit[ 1]
					; bit[ 2]
					; bit[ 3]
					; bit[ 4]
					; bit[ 5]
					; bit[ 6]
					; bit[ 7]
					; bit[ 8]
					; bit[ 9]
					; bit[10]
					; bit[11]
					; bit[12] = joystick detected
					; bit[13] = currency =	franc
					; bit[14+15]  =	mem size type?
					;	     = 0b00 = 0 (< 0x4000	free paragraphs)
					;	     = 0b01 = 1 (>= 0x4000 free paragraphs)
					;	     = 0b10 = 2 (>= 0x6000 free paragraphs)
					;	     = 0b11 = 3 (>= 0x8000 free paragraphs)
					; -------------------------

subprogram_exit_code db	0		; DATA XREF: GAME_START_sub_7+B3w

saved_video_mode db 0			; DATA XREF: start_0+89r
    
progs_cc1_filename db 'PROGS.CC1',0   

file_handle dw 0  
file_bytes_read dw 0
file_new_seek_pos dd 0
    
; =============== S U B R O U T I N E =======================================

; Macro howto
; https://www.phatcode.net/res/223/files/html/Chapter_8/CH08-7.html

PREPARE_BEFORE_CALL macro
  ; save all
  push ds
  push es
  push ax
  push bx
  push cx
  push dx
  push si
  push di
  
  ; "invalidate" all registers - is there still some hidden-in-code register in use by the called function?
  mov ax,-1
  mov es,ax
  push cs
  pop ds
  mov bx,ax
  mov cx,ax
  mov dx,ax
  mov di,ax
  mov si,ax
endm

CLEANUP_AFTER_CALL macro
  ; restore all
  pop di
  pop si
  pop dx
  pop cx
  pop bx
  pop ax
  pop es
  pop ds
endm

c_offset_overflow_safe_block_copy	proc near
  
  ; the interface
  dest_seg_ = word ptr 4
  dest_ofs_ = word ptr 6
  src_seg_ = word ptr 8
  src_ofs_ = word ptr 0Ah
  size_lo_ = word ptr 0Ch
  size_hi_ = word ptr 0Eh

  push bp
  mov bp,sp

  PREPARE_BEFORE_CALL
 
  ; set register "parameter"
  mov es,[bp+dest_seg_]
  mov di,[bp+dest_ofs_]
  mov ds,[bp+src_seg_]
  mov si,[bp+src_ofs_]
  mov cx,[bp+size_lo_]
  mov bx,[bp+size_hi_]
  
  call offset_overflow_safe_block_copy
  
  CLEANUP_AFTER_CALL

  pop bp  
  
  retn
c_offset_overflow_safe_block_copy	endp

; void __usercall offset_overflow_safe_block_copy(__int16 dest_seg_@<es>, __int16 dest_ofs_@<di>, __int16 src_seg_@<ds>, __int16 src_ofs_@<si>,	__int16	lo_size_@<cx>, __int16 hi_size_@<bx>)
offset_overflow_safe_block_copy	proc near ; CODE XREF: EXE_HEADER_sub_2+8Ep
					; EXE_HEADER_sub_2+B2p
    push  ds
    push  es
    push  si
    push  di
    push  bx
    push  cx

    cld

loc_556:				; CODE XREF: offset_overflow_safe_block_copy+47j
    ; normalize ptr
    mov ax, si
    shr ax, 1
    shr ax, 1
    shr ax, 1
    shr ax, 1
    mov dx, ds
    add dx, ax
    mov ds, dx
    and si, 0Fh
    
    ; normalize ptr
    mov ax, di
    shr ax, 1
    shr ax, 1
    shr ax, 1
    shr ax, 1
    mov dx, es
    add dx, ax
    mov es, dx
    and di, 0Fh
    
		mov	ax, 512
    sub cx, ax
    sbb bx, 0
    jnb short loc_555
    add ax, cx
    xor bx, bx
    xor cx, cx

loc_555:				; CODE XREF: offset_overflow_safe_block_copy+35j
    push  cx
    mov cx, ax
    rep movsb
    pop cx
    mov ax, cx
    or  ax, bx
    jnz short loc_556
    
    pop cx
    pop bx
    pop di
    pop si
    pop es
    pop ds
    
    retn
offset_overflow_safe_block_copy	endp


; =============== S U B R O U T I N E =======================================

; uint32_t long_mul_with_16(uint32_t value_);
long_mul_with_16 proc near

  ;
  ; ==> ax contains the lo word
  ; ==> dx contains the hi word
  ;
  ; results in a 32bit numbers in	dx:ax
  ;
  ; uint32_t result = value *	16;
  ; dx ==	(result	>> 16));
  ; ax ==	(result	& 0xFFFF));  

  ; the interface
  lo_ = word ptr 4
  hi_ = word ptr 6

  push bp
  mov bp,sp

  mov ax,[bp+lo_]  
  mov dx,[bp+hi_]  

  shl	ax, 1
  rcl dx, 1
	shl	ax, 1
  rcl dx, 1
	shl	ax, 1
  rcl dx, 1
	shl	ax, 1
	rcl	dx, 1

  ; result: dx:ax
  
  pop bp
  
  retn
long_mul_with_16 endp

; uint32_t long_div_with_16(uint32_t value_);
long_div_with_16 proc near
  ; the interface
  lo_ = word ptr 4
  hi_ = word ptr 6

  push bp
  mov bp,sp

  mov ax,[bp+lo_]  
  mov dx,[bp+hi_]  

  shr dx, 1
  rcr ax, 1
  shr dx, 1
  rcr ax, 1
  shr dx, 1
  rcr ax, 1
  shr dx, 1
  rcr ax, 1

  ; result: dx:ax
  
  pop bp
  
  retn
long_div_with_16 endp

dos_file_open proc near
  ; the interface
  filename_ = word ptr 4 ; near offset to c-str
  mode_ = word ptr 6
  handle_ = word ptr 8 ; near offset to handle variable
  
  ; ax = result code 

  push bp
  mov bp,sp

  push dx
  push bx

  mov ah,3Dh
  mov al,byte ptr [bp+mode_]
  mov dx,[bp+filename_]  
  int 21h
  jc exit ; ax = error code
  
  mov bx,[bp+handle_]
  mov cs:[bx],ax
  mov ax,0

exit:
  pop bx
  pop dx
  pop bp
  
  retn
dos_file_open endp

dos_file_read proc near
  ; the interface
  file_handle_ = word ptr 4 ; value not offset
  byte_to_read_ = word ptr 6
  buffer_seg_ = word ptr 8
  buffer_ofs_ = word ptr 0Ah
  bytes_read_ = word ptr 0Ch
  
  ; ax = result code 

  push bp
  mov bp,sp

  push ds
  push bx
  push cx
  push dx

  mov ah,3Fh
  mov bx,[bp+file_handle_]  
  mov cx,[bp+byte_to_read_]
  mov ds,[bp+buffer_seg_]
  mov dx,[bp+buffer_ofs_]
  int 21h
  jc exit ; ax = error code
  
  mov bx,[bp+bytes_read_]
  mov cs:[bx],ax
  mov ax,0

exit:
  pop dx
  pop cx
  pop bx
  pop ds
  pop bp
  
  retn
dos_file_read endp

dos_file_lseek proc near
  ; the interface
  file_handle_ = word ptr 4 ; value not offset
  mode_ = word ptr 6
  offset_lo_ = word ptr 8
  offset_hi_ = word ptr 0Ah
  new_offset_ = word ptr 0Ch
  
  ; ax = result code 

  push bp
  mov bp,sp

  push bx
  push cx
  push dx

  mov ah,42h
  mov al,byte ptr [bp+mode_]
  mov bx,[bp+file_handle_]  
  mov cx,[bp+offset_hi_]
  mov dx,[bp+offset_lo_]
  int 21h
  jc exit ; ax = error code

  mov bx,[bp+new_offset_]
  mov cs:[bx+0],ax
  mov cs:[bx+2],dx
  mov ax,0

exit:
  pop dx
  pop cx
  pop bx

  pop bp
  
  retn
dos_file_lseek endp


dos_file_close proc near
  ; the interface
  file_handle_ = word ptr 4 ; value not offset
  
  ; ax = result code 

  push bp
  mov bp,sp

  push dx
  push bx

  mov ah,3Eh
  mov bx, [bp+file_handle_]
  int 21h
  jc exit ; ax = error code
 
  mov ax,0

exit:
  pop bx
  pop dx
  pop bp
  
  retn
dos_file_close endp

; normalize_ptr proc near
;   ; the interface
;   seg_ = word ptr 4
;   ofs_ = word ptr 6
;   
;   push bp
;   mov bp,sp
; 
;   mov ax,[bp+ofs_]
;   push ax
;   mov dx,[bp+seg_]
; 
;   shr ax, 1
;   shr ax, 1
;   shr ax, 1
;   shr ax, 1
;   add dx, ax
;   pop ax
;   and ax, 0Fh
;   
;   ;dx:ax -> seg:ofs
;   
;   retn
; normalize_ptr endp

; __int16 __usercall __far EXE_HEADER_sub_2<ax>(__int16	unknown1_@<ax>,	__int16	unknown2_@<dx>,	__int16	unknown3_@<cx>,	__int16	unknown4_@<bx>,	__int16	unknown5_@<ds>,	__int16	unknown6_@<si>,	__int16	unknown6_@<es>,	__int16	unknown7_@<di>)
EXE_HEADER_sub_2 proc far		; CODE XREF: GAME_START_sub_7+9Bp
    push  bx
    test  cs:byte_55, 0C0h
    jnz short loc_557
    xor ax, ax
    clc
    pop bx
		retn			; near ret from	far proc - manually fixed stack?
; ---------------------------------------------------------------------------

loc_557:				; CODE XREF: EXE_HEADER_sub_2+7j
		mov	cs:word_558, ax	; something with someway_cs_registe_value1 - lea instruction
		mov	cs:some_register_cs_value, dx ;	= cs
		mov	cs:word_62, cx	; = some_game_ptr.ofs
		mov	cs:some_game_pointer_seg, bx ; = some_game_ptr.segm
		mov	cs:exe_pointer.ofs, si ; = 0
		mov	cs:exe_pointer.segm, ds	; = somway_exe_buffer_seg + 10h
    mov ah, 50h
		mov	bx, cs:new_psp_seg
    int 21h   ; DOS - 2+ internal - SET PSP SEGMENT
          ; BX = segment address of new PSP
    push  es
    push  ax
    push  ds
    xor ax, ax
    mov es, ax
    assume es:nothing
		lds	ax, dword ptr cs:saved_int1_ptr.ofs
    mov es:4, ax
    mov word ptr es:6, ds
		pop	ds		; src_seg_@
    pop ax
		pop	es		; dest_seg_@
    assume es:nothing
    cld
		mov	cs:pointer3.ofs, di
		mov	cs:pointer3.segm, es
		lds	si, dword ptr cs:exe_pointer.ofs ; unknown_@   .ofs seems wrong???
    test  cs:byte_55, 40h
    jz  short loc_560
    jmp loc_561
; ---------------------------------------------------------------------------

loc_560:				; CODE XREF: EXE_HEADER_sub_2+62j
		cmp	[si+EXE_Header.signature], 'ZM' ; 'MZ' or 'ZM' exe header signature??
    jz  short loc_562
    stc
    pop bx
		retn			; near ret from	far proc - manually fixed stack?
; ---------------------------------------------------------------------------

loc_562:				; CODE XREF: EXE_HEADER_sub_2+6Bj
    ;----
    push dx
    push 0 ; = hi
    push [si+EXE_Header.headersize] ; = lo
    call long_mul_with_16
    add sp,2*2
    ; dx:ax
    mov cx,ax ; = lo
    mov bx,dx ; = hi
    pop dx
    ;----

		les	di, dword ptr cs:pointer3.ofs ;	dest_ofs_@
    push bx ; hi
    push cx ; low
    push si
    push ds
    push di
    push es
    call c_offset_overflow_safe_block_copy
    add sp,6*2
    
    mov dx, bx
		mov	si, cx		; src_ofs_@
		mov	bx, cs:some_game_pointer_seg
    mov cx, cs:word_62
		sub	cx, si		; lo_size_@
		sbb	bx, dx		; hi_size_@
    mov cs:some_game_pointer_seg, bx
    mov cs:word_62, cx
		les	di, dword ptr cs:exe_pointer.ofs ; dest_ofs_@
   
    push bx ; hi
    push cx ; lo
    push si
    push ds
    push di
    push es
    call c_offset_overflow_safe_block_copy
    add sp,6*2
    
    mov bp, es
		les	bx, dword ptr cs:pointer3.ofs
    mov cx, es:[bx+6]
    jcxz  short loc_563
    mov di, es:[bx+18h]
    add di, bx

loc_564:				; CODE XREF: EXE_HEADER_sub_2+D8j
    mov si, es:[di]
    mov ax, es:[di+2]
    add ax, bp
    mov ds, ax
    add [si], bp
    add di, 4
    loop  loc_564

loc_563:				; CODE XREF: EXE_HEADER_sub_2+C0j
		les	si, dword ptr cs:pointer3.ofs
		mov	ax, es:[si+EXE_Header.initSP]
		mov	cs:register_sp_value, ax
		mov	ax, es:[si+EXE_Header.initSS]
    add ax, bp
		mov	cs:register_ss_value, ax
		mov	ax, es:[si+EXE_Header.initCS]
    add ax, bp
		mov	cs:exe_cs_ip_ptr.segm, ax
		mov	ax, es:[si+EXE_Header.initIP]
		mov	cs:exe_cs_ip_ptr.ofs, ax
		mov	es, cs:new_psp_seg
    cld
    mov di, es
		mov	ax, cs:register_sp_value
		mov	dx, cs:register_ss_value
    mov cx, cs:word_62
		mov	bx, cs:some_game_pointer_seg
    cli
		mov	ss, dx		;
					;
					; --
					;
					; prepare registers for	exe start
					;
					;
					; --
    mov sp, ax
    mov ds, di
    xor ax, ax
    push  ax
    sti
		jmp	dword ptr cs:exe_cs_ip_ptr.ofs ; jump into game	code
; ---------------------------------------------------------------------------

loc_561:				; CODE XREF: EXE_HEADER_sub_2+64j
    
    cli
		
		mov	ax, cs:new_psp_seg
    mov es, ax
    mov ds, ax
    mov ss, ax
    mov sp, 0FFFFh
    xor bx, bx
    push  bx
    push  ax
		mov	ax, size sPSP	; maybe	PSP size
    push  ax
    mov ax, bx
    mov cx, bx
    mov dx, bx
    mov bp, bx
    mov si, bx
    mov di, bx
    
    sti
    
    retf
EXE_HEADER_sub_2 endp


; =============== S U B R O U T I N E =======================================

; __int16 __usercall GAME_START_sub_3<ax>(__int16 maybe_dest_seg_@<es>,	__int16	maybe_dest_ofs1_@<di>, __int16 maybe_src_seg_@<ds>, __int16 maybe_src_ofs_@<si>)

c_GAME_START_sub_3	proc near
  
  ; the interface
  es_seg_ = word ptr 4
  di_ofs_ = word ptr 6
  ds_seg_ = word ptr 8
  si_ofs_ = word ptr 0Ah

  push bp
  mov bp,sp

  PREPARE_BEFORE_CALL
 
  ; set register "parameter"
  mov es,[bp+es_seg_]
  mov di,[bp+di_ofs_]
  mov ds,[bp+ds_seg_]
  mov si,[bp+si_ofs_]
  
  call GAME_START_sub_3
  
  CLEANUP_AFTER_CALL

  pop bp  
  
  retn
c_GAME_START_sub_3 endp

extern C cc_GAME_START_sub_3:near
; Info: does not call other functions
; __int16 __usercall GAME_START_sub_3<ax>(__int16 maybe_dest_seg_@<es>,	__int16	maybe_dest_ofs1_@<di>, __int16 maybe_src_seg_@<ds>, __int16 maybe_src_ofs_@<si>)
GAME_START_sub_3 proc near		; CODE XREF: GAME_START_sub_3+101j
          ; read_some_file_sub_4+1EDp
sub_3_begin:    
    push  es
    push  di
      mov	cx, 128
      mov ax, ds
      mov es, ax
      assume es:seg000
      mov	di, 301h	; its not in seg000!!!
      xor ax, ax
      rep stosw
      ;mem_set(ds:301h, 0, 128*2=512);
    pop di
    pop es
    assume es:nothing
    
    push  es
      xor ax, ax
      mov es, ax
      assume es:nothing
      sub di, es:(1*sizeof ptr16)+ptr16.ofs ; interrupt[1].ofs
    pop es
    assume es:nothing
    
    ; normalize ptr
    mov ax, di
    shr ax, 1
    shr ax, 1
    shr ax, 1
    shr ax, 1
    mov cx, es
    add cx, ax
    mov es, cx
    and di, 0Fh
    
    push  es
      xor ax, ax
      mov es, ax
      assume es:nothing
      add di, es:(1*sizeof ptr16)+ptr16.ofs ; interrupt[1].ofs
    pop es
    assume es:nothing

    push  ds
    push  es
    push  si
    push  di

      mov cx, 4
      mov	di, offset byte_57 ; byte_57 holds 4 bytes - see rep movsb
      mov ax, cs
      mov es, ax
      assume es:seg000
      lds	si, dword ptr cs:another_pointer2.ofs
      
      ; normalize ptr
      mov ax, si
      shr ax, 1
      shr ax, 1
      shr ax, 1
      shr ax, 1
      mov dx, ds
      add ax, dx
      mov ds, ax
      and si, 0Fh
      
      ; INFO: 
      ; https://www.javatpoint.com/near-far-and-huge-pointers-in-c
      ; The huge pointer can be incremented without suffering with segment work round. __huge
      ; char huge * far *p; 
      
      mov	cs:another_pointer2.ofs, si
      mov	cs:another_pointer2.segm, ds
      add	cs:another_pointer2.ofs, cx
      rep movsb
    
    pop di
    pop si
    pop es
    assume es:nothing
    pop ds
    
    mov dx, cs:word_60
    inc dx
    cmp cs:byte_57, 0
    jnz short loc_565
    jmp loc_566
; ---------------------------------------------------------------------------

loc_565:				; CODE XREF: GAME_START_sub_3+82j
    push  ds
    push  es
    push  di
    xor ch, ch
    mov cl, cs:byte_57
    mov di, 201h
    mov ax, ds
    mov es, ax
    assume es:seg000
		mov	ds, cs:another_pointer2.segm
		mov	si, cs:another_pointer2.ofs
		add	cs:another_pointer2.ofs, cx
    rep movsb
    mov cl, cs:byte_57
    xor ch, ch
    mov di, 1
		add	cs:another_pointer2.ofs, cx
    rep movsb
    mov cl, cs:byte_57
    mov di, 101h
		add	cs:another_pointer2.ofs, cx
    rep movsb
		pop	di		; maybe_dest_ofs1_@
		pop	es		; maybe_dest_seg_@
    assume es:nothing
		pop	ds		; maybe_src_seg_@
    xor ch, ch
    mov cl, cs:byte_57
    xor ah, ah
    mov bx, 1

loc_567:				; CODE XREF: GAME_START_sub_3+EBj
    mov al, [bx+200h]
		mov	si, ax		; maybe_src_ofs_@
    mov dl, [si+301h]
    mov [bx+402h], dl
    mov [si+301h], bl
    inc bx
    loop  loc_567
    mov dx, cs:word_60
    inc dx
    mov cx, 1

loc_124:				; CODE XREF: GAME_START_sub_3+123j
					; GAME_START_sub_3+16Ej
    dec dx
    jnz short loc_568

loc_577:				; CODE XREF: GAME_START_sub_3+192j
    cmp cs:byte_569, 0
    jz  short locret_570
		jmp	sub_3_begin
; ---------------------------------------------------------------------------

locret_570:				; CODE XREF: GAME_START_sub_3+FFj
    retn
; ---------------------------------------------------------------------------

loc_568:				; CODE XREF: GAME_START_sub_3+F7j
    push  ds
		mov	si, cs:another_pointer2.segm
    mov ds, si
		mov	si, cs:another_pointer2.ofs
    lodsb
		mov	cs:another_pointer2.ofs, si
    pop ds
    mov bx, ax
    cmp byte ptr [bx+301h], 0
    jnz short loc_571
    stosb
    jmp short loc_124
; ---------------------------------------------------------------------------

loc_571:				; CODE XREF: GAME_START_sub_3+120j
    mov bl, [bx+301h]
    xor ax, ax
    push  ax
    jmp short loc_128
; ---------------------------------------------------------------------------

loc_129:				; CODE XREF: GAME_START_sub_3+164j
					; GAME_START_sub_3+174j
    mov bp, ax
    cmp byte ptr ds:[bp+301h], 0
    jz  short loc_572
    cmp bl, ds:[bp+301h]
    ja  short loc_573
    mov al, bl
    mov bl, ds:[bp+301h]

loc_575:				; CODE XREF: GAME_START_sub_3+152j
    mov bl, [bx+402h]
    or  bl, bl
    jz  short loc_574
    cmp bl, al
    jb  short loc_128
    jmp short loc_575
; ---------------------------------------------------------------------------

loc_573:				; CODE XREF: GAME_START_sub_3+13Dj
    mov bl, ds:[bp+301h]

loc_128:				; CODE XREF: GAME_START_sub_3+12Cj
					; GAME_START_sub_3+150j
    mov al, [bx+100h]
    mov ah, bl
    push  ax
    xor ah, ah
    mov al, [bx]
    jmp short loc_129
; ---------------------------------------------------------------------------

loc_574:				; CODE XREF: GAME_START_sub_3+14Cj
    mov ax, bp

loc_572:				; CODE XREF: GAME_START_sub_3+136j
    stosb
    pop ax
    or  ax, ax
    jnz short loc_576
    jmp short loc_124
; ---------------------------------------------------------------------------

loc_576:				; CODE XREF: GAME_START_sub_3+16Cj
    mov bl, ah
    xor ah, ah
    jmp short loc_129
; ---------------------------------------------------------------------------

loc_566:				; CODE XREF: GAME_START_sub_3+84j
    push  ds
    push  es
    mov cx, cs:word_60
    push  cx
		mov	ds, cs:another_pointer2.segm
		mov	si, cs:another_pointer2.ofs
		add	cs:another_pointer2.ofs, cx
    rep movsb
    pop cx
    pop es
    pop ds
    jmp loc_577
GAME_START_sub_3 endp ;	sp-analysis failed


; =============== S U B R O U T I N E =======================================

c_read_some_file_sub_4	proc near
  
  ; the interface
  block_ = word ptr 4 

  push bp
  mov bp,sp
  
  PREPARE_BEFORE_CALL
 
  ; set register "parameter"
  mov bx,[bp+block_]
  
  call read_some_file_sub_4

  CLEANUP_AFTER_CALL

  pop bp  
  
  retn
c_read_some_file_sub_4	endp

; __int16 __usercall read_some_file_sub_4<ax>(gfx_block_t *block_@<bx>)
read_some_file_sub_4 proc near		; CODE XREF: GAME_START_sub_7+2Ap
    ; bx = offset 0 of gfx block == filename == always progs.cc1
    mov ax, cs
    mov ds, ax

    ; its always "progs.cc1"
    mov dx, offset progs_cc1_filename
    
    mov ah, 3Dh
    mov al, 0
    int 21h   ; DOS - 2+ - OPEN DISK FILE WITH HANDLE
          ; DS:DX -> ASCIZ filename
          ; AL = access mode
          ; 0 - read
    jnb short loc_578

loc_136:        ; CODE XREF: read_some_file_sub_4+45j
          ; read_some_file_sub_4+4Aj ...
    stc
    retn
; ---------------------------------------------------------------------------

loc_578:        ; CODE XREF: read_some_file_sub_4+Cj
    mov si, bx ; block start
    mov bx, ax
    mov ah, 3Fh
    mov cl, cs:byte_55
    test  cl, 18h
    jnz short loc_579
		lds	cx, dword ptr cs:maybe_exe_buffer.ofs
    mov cs:word_44, 0FFFFh
    mov cs:word_45, 0FFFFh
    jmp loc_580
; ---------------------------------------------------------------------------

loc_579:        ; CODE XREF: read_some_file_sub_4+1Ej
    test  cl, 10h
    jz  short loc_581
    mov cx, 2
		lds	dx, dword ptr cs:maybe_exe_buffer.ofs
    int 21h   ; DOS - 2+ - READ FROM FILE WITH HANDLE
          ; BX = file handle, CX = number of bytes to read
          ; DS:DX -> buffer
    jb  short loc_136
    cmp  ax, 2
    jnz short loc_136
    mov di, dx
    mov cx, [di]
    xchg  cl, ch
    mov di, cx
		mov	al, cs:[si+gfx_block_t.byte_12h]
    xor ah, ah
    shl ax, 1
    shl ax, 1
    mov dx, ax
    xor cx, cx

IF 1
    mov al, 1
    mov ah, 42h
    int 21h   ; DOS - 2+ - MOVE FILE READ/WRITE POINTER (LSEEK)
          ; AL = method: offset from present location
    jb  short loc_136
ELSE
    ; TODO BUG crashes!!!
    push ax
    
    push offset file_new_seek_pos
    push dx
    push cx
    push 1
    push bx    
    call dos_file_lseek
    add sp,5*2
    
    pop ax
ENDIF    
    
		lds	dx, dword ptr cs:maybe_exe_buffer.ofs
    mov ah, 3Fh
    mov cx, 4
    int 21h   ; DOS - 2+ - READ FROM FILE WITH HANDLE
          ; BX = file handle, CX = number of bytes to read
          ; DS:DX -> buffer
    jb  short loc_136
    cmp  ax, 4
    jnz short loc_136
    mov si, dx
    mov cx, [si]
    xchg  cl, ch
    mov dx, [si+2]
    xchg  dl, dh
    shl di, 1
    shl di, 1
    add di, 2
    add dx, di
    adc cx, 0
    mov al, 0
    mov ah, 42h
    int 21h   ; DOS - 2+ - MOVE FILE READ/WRITE POINTER (LSEEK)
          ; AL = method: offset from beginning of file
    jb  short loc_139

loc_581:        ; CODE XREF: read_some_file_sub_4+39j
		lds	dx, dword ptr cs:maybe_exe_buffer.ofs
    mov ah, 3Fh
    mov cx, 8
    int 21h   ; DOS - 2+ - READ FROM FILE WITH HANDLE
          ; BX = file handle, CX = number of bytes to read
          ; DS:DX -> buffer
    jb  short loc_139
    cmp ax, 8
    jz  short loc_582

loc_139:        ; CODE XREF: read_some_file_sub_4+9Aj
          ; read_some_file_sub_4+A8j
    stc
    retn
; ---------------------------------------------------------------------------

loc_582:        ; CODE XREF: read_some_file_sub_4+ADj
		lds	bp, dword ptr cs:maybe_exe_buffer.ofs
    mov ax, ds:[bp+0]
    mov cx, ds:[bp+2]
    xchg  al, ah
    xchg  cl, ch
    mov cs:word_44, cx
    mov cs:word_45, ax
    mov ax, ds:[bp+4]
    mov cx, ds:[bp+6]
    xchg  al, ah
    xchg  cl, ch
		mov	cs:some_game_ptr.ofs, cx
		mov	cs:some_game_ptr.segm, ax
    mov si, cx
    mov di, ax
		lds	cx, dword ptr cs:maybe_exe_buffer.ofs
    add si, cx
    adc di, 0
    sub si, cs:word_44
    sbb di, cs:word_45
    add si, 10h
    adc di, 0
    mov cx, si
    
IFNDEF LONG_DIV_MUL
    shr di, 1
    rcr si, 1
    shr di, 1
    rcr si, 1
    shr di, 1
    rcr si, 1
    shr di, 1
    rcr si, 1
ELSE
    ;----
    push ax 
    push dx
    
    push di ; = hi
    push si ; = lo
    call long_div_with_16
    add sp,2*2
    ; dx:ax
    mov si,ax ; = lo
    mov di,dx ; = hi
    
    pop dx
    pop ax
    ;----
ENDIF    
    ; some sort of normalize ptr at end
    mov ax, ds
    add ax, si
    mov ds, ax
    and cx, 0Fh

loc_580:        ; CODE XREF: read_some_file_sub_4+33j
		mov	cs:another_far_ptr.ofs,	cx
		mov	cs:another_far_ptr.segm, ds
		mov	cs:another_pointer2.ofs, cx
		mov	cs:another_pointer2.segm, ds
    mov si, cs:word_44
    mov di, cs:word_45
		mov	cs:also_a_pointer.ofs, 0
		mov	cs:also_a_pointer.segm,	0
		lds	dx, dword ptr cs:another_far_ptr.ofs

loc_586:        ; CODE XREF: read_some_file_sub_4+18Dj
    ; normalize ptr
    mov cx, dx
    shr cx, 1
    shr cx, 1
    shr cx, 1
    shr cx, 1
    mov ax, ds
    add ax, cx
    mov ds, ax
    and dx, 0Fh
    
		mov	ax, 48000
    sub si, ax
    sbb di, 0
    jnb short loc_583
    add si, ax
    mov ax, si
    xor si, si
    xor di, di

loc_583:        ; CODE XREF: read_some_file_sub_4+165j
    mov cx, ax
    mov ah, 3Fh
    int 21h   ; DOS - 2+ - READ FROM FILE WITH HANDLE
          ; BX = file handle, CX = number of bytes to read
          ; DS:DX -> buffer
    jnb short loc_584
    retn
; ---------------------------------------------------------------------------

loc_584:        ; CODE XREF: read_some_file_sub_4+175j
		add	cs:also_a_pointer.ofs, ax
		adc	cs:also_a_pointer.segm,	0
    add dx, ax
    cmp ax, cx
    jnz short loc_585
    mov ax, si
    or  ax, di
    jnz short loc_586

loc_585:        ; CODE XREF: read_some_file_sub_4+187j
    mov ah, 3Eh
    int 21h   ; DOS - 2+ - CLOSE A FILE WITH HANDLE
          ; BX = file handle
    test  cs:byte_55, 18h
    jz  short loc_587
		les	di, dword ptr cs:maybe_exe_buffer.ofs ;	maybe_dest_ofs1_@
    push  es
    xor ax, ax
    mov es, ax
    assume es:nothing
    mov es:4, di
		pop	es		; maybe_dest_seg_@
    assume es:nothing
		mov	cs:another_far_ptr.ofs,	di
		mov	cs:another_far_ptr.segm, es
		lds	si, dword ptr cs:some_game_ptr.ofs
    mov cx, ds
    add si, 10h
    add cx, 0
    add si, di
    adc cx, 0
    mov bx, si
    
IFNDEF LONG_DIV_MUL
    shr cx, 1
    rcr si, 1
    shr cx, 1
    rcr si, 1
    shr cx, 1
    rcr si, 1
    shr cx, 1
		rcr	si, 1		; maybe_src_ofs_@
ELSE
    ;----
    push ax
    push dx
    
    push cx ; = hi
    push si ; = lo
    call long_div_with_16
    add sp,2*2
    ; dx:ax
    mov cx,dx ; = hi
    mov si,ax ; = lo
    
    pop dx
    pop ax  
    ;----
ENDIF      
    
    mov ax, es
    add ax, si
		mov	cs:maybe_exe_buffer.segm, ax
    inc ax
		mov	ds, ax		; maybe_src_seg_@
    and bx, 0Fh
		mov	cs:maybe_exe_buffer.ofs, bx
    cld
   
    push si
    push ds
    push di
    push es
IF 0
    call c_GAME_START_sub_3
ELSE
    call cc_GAME_START_sub_3
ENDIF    
    add sp,4*2
  
    clc
    retn
; ---------------------------------------------------------------------------

loc_587:        ; CODE XREF: read_some_file_sub_4+199j
		lds	si, dword ptr cs:also_a_pointer.ofs
		mov	cs:some_game_ptr.ofs, si
		mov	cs:some_game_ptr.segm, ds
    clc
    retn
read_some_file_sub_4 endp


; =============== S U B R O U T I N E =======================================

c_GAME_START_sub_5	proc near
  
  ; the interface
  ; NONE

  push bp
  mov bp,sp
  
  PREPARE_BEFORE_CALL
 
  ; set register "parameter"
  ; NONE
  
  call GAME_START_sub_5

  CLEANUP_AFTER_CALL

  pop bp  
  
  retn
c_GAME_START_sub_5	endp

GAME_START_sub_5 proc near		; CODE XREF: GAME_START_sub_7+B7p
    push  bx
    mov al, cs:byte_55
    test  al, 20h
    jz  short loc_588
    test  al, 10h
    jnz short loc_589
		mov	ax, cs:somway_exe_buffer_seg
    mov ds, ax
    mov es, ax
    mov bp, ax
    xor si, si
    xor di, di
    mov cx, [si]
    shr cx, 1
    shr cx, 1

loc_590:				; CODE XREF: GAME_START_sub_5+41j
    mov ax, [si]
    mov bx, [si+2]
    
IFNDEF LONG_DIV_MUL
    shr bx, 1
    rcr ax, 1
    shr bx, 1
    rcr ax, 1
    shr bx, 1
    rcr ax, 1
    shr bx, 1
    rcr ax, 1
ELSE
    ;----
    push dx
    
    push bx ; = hi
    push ax ; = lo
    call long_div_with_16
    add sp,2*2
    ; dx:ax
    mov bx,dx ; = hi
    
    pop dx
    ;----
ENDIF     
    
    add ax, bp
    mov [si+2], ax
    and word ptr [si], 0Fh
    add si, 4
    loop  loc_590

loc_589:				; CODE XREF: GAME_START_sub_5+Bj
    mov al, cs:byte_55
    test  al, 0C0h
    jnz short loc_591
		les	bx, dword ptr cs:far_ptr3.ofs
    mov ax, es
    
IFNDEF LONG_DIV_MUL
    shr ax, 1
    rcr bx, 1
    shr ax, 1
    rcr bx, 1
    shr ax, 1
    rcr bx, 1
    shr ax, 1
    rcr bx, 1
ELSE
    ;----
    push dx
    
    push ax ; = hi
    push bx ; = lo
    call long_div_with_16
    add sp,2*2
    ; dx:ax
    mov bx,ax ; = lo
    mov ax,dx ; = hi
    
    pop dx
    ;----
ENDIF    
    
    inc bx
		mov	es, cs:somway_exe_buffer_seg
    mov ah, 4Ah
    int 21h   ; DOS - 2+ - ADJUST MEMORY BLOCK SIZE (SETBLOCK)
          ; ES = segment address of block to change
          ; BX = new size in paragraphs

loc_591:				; CODE XREF: GAME_START_sub_5+49j
    clc
    pop bx
    retn
; ---------------------------------------------------------------------------

loc_588:				; CODE XREF: GAME_START_sub_5+7j
    pop bx
    retn
GAME_START_sub_5 endp


; =============== S U B R O U T I N E =======================================

c_GAME_START_sub_6	proc near
  
  ; the interface
  ; NONE

  push bp
  mov bp,sp
  
  PREPARE_BEFORE_CALL
 
  ; set register "parameter"
  ; NONE
  
  call GAME_START_sub_6

  CLEANUP_AFTER_CALL

  pop bp  
  
  retn
c_GAME_START_sub_6	endp

REMOVE_DEAD_CODE = 1 ; test with all variants

GAME_START_sub_6 proc near		; CODE XREF: GAME_START_sub_7+25p
    push  bx
    xor ax, ax
		mov	cs:far_ptr3.ofs, ax
		mov	cs:far_ptr3.segm, ax

    mov al, cs:byte_55
    test  al, 20h
IFDEF REMOVE_DEAD_CODE    
    jz next
    mov dx,offset error9
    mov ah,09h
    int 21h
    jmp just_exit
next:    
ELSE
    jnz short loc_592
ENDIF    

		mov	cs:far_ptr3.ofs, size sPSP
		mov	dx, cs:maybe_exe_buffer.segm
		add	cs:maybe_exe_buffer.segm, 16
		mov	cs:new_psp_seg,	dx
    mov ah, 26h
    int 21h   ; DOS - CREATE PSP
          ; DX = segment number at which to set up PSP
		mov	dx, sPSP.cmdtail_or_dta	; dta_offs
		mov	ds, cs:new_psp_seg
    mov ah, 1Ah
    int 21h   ; DOS - SET DISK TRANSFER AREA ADDRESS
          ; DS:DX -> disk transfer buffer
		mov	word ptr ds:sPSP.int_22.segm, cs
		mov	word ptr ds:sPSP.int_22, offset	subprogram_exit	; int22	exit target
    clc
    pop bx
    retn
    
IFNDEF REMOVE_DEAD_CODE     
; ---------------------------------------------------------------------------

loc_592:				; CODE XREF: GAME_START_sub_6+11j
    xor ax, ax
    mov ds, ax
    assume ds:nothing
    lea ax, ds:((90*sizeof ptr16)+1) ; ??? Interrupt[90]+1 ???
    ; Interrupt[155]
    mov ds:(155*sizeof ptr16+ptr16.ofs), ax 
    mov word ptr ds:(155*sizeof ptr16+ptr16.segm), cs
    
    ;====
    
		mov	cx, lengthof maybe_10_ptr ; item count not bytes
    mov ax, cs
    mov ds, ax
    assume ds:seg000
		lea	si, maybe_10_ptr

loc_594:				; CODE XREF: GAME_START_sub_6+6Bj
		mov	ax, [si+ptr16.ofs]
		or	ax, [si+ptr16.segm]
    jz  short loc_593
    add si, size ptr16
    loop  loc_594
    stc
    pop bx
    retn
; ---------------------------------------------------------------------------

loc_593:				; CODE XREF: GAME_START_sub_6+66j
		mov	ax, cs:maybe_exe_buffer.segm
		mov	[si+ptr16.segm], ax
		mov	[si+ptr16.ofs],	0
    clc
    pop bx
    retn
ENDIF    
GAME_START_sub_6 endp


; =============== S U B R O U T I N E =======================================
    
c_GAME_START_sub_7	proc near
  
  ; the interface
  block_ = word ptr 4 

  push bp
  mov bp,sp
  
  PREPARE_BEFORE_CALL
 
  ; set register "parameter"
  mov bx,[bp+block_]
  
  call GAME_START_sub_7

  CLEANUP_AFTER_CALL

  pop bp  
  
  retn
c_GAME_START_sub_7	endp    
    
; __int16 __usercall GAME_START_sub_7<ax>(gfx_block_t *block_@<bx>)
GAME_START_sub_7 proc near		; CODE XREF: START_GAME_sub_22:loc_655p
    push  bx
    
    push  bx
    mov ah, 48h
    mov bx, 0FFFFh
    int 21h   ; DOS - 2+ - ALLOCATE MEMORY
          ; BX = number of 16-byte paragraphs desired
					; AX = segment of reserved block
					;
					; this call always fails (CF=1,	ax=8, bx=max available blocks)
    mov ah, 48h
    int 21h   ; DOS - 2+ - ALLOCATE MEMORY
          ; BX = number of 16-byte paragraphs desired
					; AX = segment of reserved block
		mov	cs:maybe_exe_buffer.segm, ax
		mov	cs:somway_exe_buffer_seg, ax
		mov	cs:maybe_exe_buffer.ofs, 0
    pop	bx		; block_@
    
		mov	al, cs:[bx+gfx_block_t.byte_13h]
    mov cs:byte_55, al

    call c_GAME_START_sub_6
    
    jb  short loc_595

    push bx
    call  c_read_some_file_sub_4
    add sp,1*2
    
    jnb short loc_596

loc_595:				; CODE XREF: GAME_START_sub_7+28j
    stc
    pop bx
    retn
; ---------------------------------------------------------------------------

loc_596:				; CODE XREF: GAME_START_sub_7+2Dj
		les	si, dword ptr cs:some_game_ptr.ofs
    mov di, es
		add	cs:far_ptr3.ofs, si
		adc	cs:far_ptr3.segm, di
		les	si, dword ptr cs:far_ptr3.ofs
    mov di, es
		mov	ax, cs:somway_exe_buffer_seg

IFNDEF LONG_DIV_MUL
    shr di, 1
    rcr si, 1
    shr di, 1
    rcr si, 1
    shr di, 1
    rcr si, 1
    shr di, 1
    rcr si, 1
ELSE
    ;----
    ; si:di = si:di / 16 
    push ax 
    push dx
    
    push di ; = hi
    push si ; = lo
    call long_div_with_16
    add sp,2*2
    ; dx:ax
    mov si,ax ; = lo
    mov di,dx ; = hi
    
    pop dx
    pop ax
    ;----
ENDIF  
    
    inc si
    add ax, si
    mov es, ax
    xor di, di
		lds	cx, dword ptr cs:some_game_ptr.ofs ; unknown3_@
		mov	bx, ds		; unknown4_@
		mov	ax, cs:somway_exe_buffer_seg
    		add	ax, 16		; seg +	16 == 16*16 bytes = 256(100h) bytes move - PSP size?
		mov	ds, ax		; unknown5_@
		xor	si, si		; unknown6_@
		mov	dx, cs		; unknown2_@
		mov	cs:someway_cs_registe_value1, dx
		mov	cs:someway_cs_registe_value2, dx
		mov	cs:someway_cs_registe_value3, dx
		mov	cs:someway_cs_registe_value4, dx
		lea	ax, someway_cs_registe_value1 ;	unknown1_@
		mov	cs:some_register_sp_value, sp
		mov	cs:some_register_ss_value, ss
		call	near ptr EXE_HEADER_sub_2 ; !!!!calls the game code
		jnb	short subprogram_exit
    stc
    pop bx
    retn
; ---------------------------------------------------------------------------

IFDEF __WASM__
subprogram_exit:			; CODE XREF: GAME_START_sub_7+9Ej
ELSE
subprogram_exit::			; CODE XREF: GAME_START_sub_7+9Ej
ENDIF
					; DATA XREF: GAME_START_sub_6+3Eo
    cli
		mov	sp, cs:some_register_sp_value
		mov	ss, cs:some_register_ss_value
    sti
    mov ah, 4Dh
    int 21h   ; DOS - 2+ - GET EXIT CODE OF SUBPROGRAM (WAIT)
		mov	cs:subprogram_exit_code, al

    call c_GAME_START_sub_5
    
    pop bx
    retn
GAME_START_sub_7 endp ;	sp-analysis failed


; =============== S U B R O U T I N E =======================================

; Attributes: noreturn

start_0   proc near   ; CODE XREF: startj

    ; prepare registers, stack etc.
    cld
    
    ;==================
    cli
    
    mov ax, cs
    mov ss, ax
    assume ss:seg000
    mov ds, ax
    mov es, ax
    assume es:seg000
    mov sp, offset stack_space_end_unk_342

    ; get inital PSP
    mov ah, 51h
    int 21h   ; DOS - 2+ internal - GET PSP SEGMENT
          ; Return: BX = current PSP segment
    mov cs:start_psp, bx
    
    sti
    ;==================
    
    call SIMPLE_INIT_routine
   
    call  read_config_and_resize_memory ; CF = 1 on error
    jb  short shutdown_cleanup ; shutdown on error

    ; former menu screen start loop
    
;====== ??? free game memory for restart? im currently missing the free part due to menu cycle change
; TODO split first start and cleanup - move cleanup to shutdown

		lea	ax, maybe_10_ptr
		mov	cx, lengthof maybe_10_ptr ; item count not bytes ; WASM sets 1Eh, UASM/MASM 0Ah
    mov ax, cs
    mov ds, ax
		lea	si, maybe_10_ptr

loc_844:				; CODE XREF: start_0+53j
    les ax, [si]
    assume es:nothing
    mov bx, es
    or  ax, bx
		jz	short loc_843
    mov ah, 49h
    int 21h   ; DOS - 2+ - FREE MEMORY
          ; ES = segment address of area to be freed

loc_843:				; CODE XREF: start_0+4Aj
		add	si, size ptr16
		loop	loc_844
		lea	di, maybe_10_ptr
    mov ax, cs
    mov es, ax
    assume es:seg000
    xor ax, ax

		mov	cx, sizeof maybe_10_ptr ; WASM sets cx wrongly to 78h, UASM/MASM are ok
    rep stosw
;======    

    jmp start_game
; ---------------------------------------------------------------------------

IFDEF __WASM__
shutdown_cleanup:
ELSE
shutdown_cleanup::
ENDIF

    ; restore video mode
    xor ah, ah
		mov	al, cs:saved_video_mode
    int 10h   ; - VIDEO - SET VIDEO MODE
          ; AL = mode
          
    ; restore PSP
    mov bx, cs:start_psp
    mov ah, 50h
    int 21h   ; DOS - 2+ internal - SET PSP SEGMENT
          ; BX = segment address of new PSP

    ; restore int1
    push  es
    push  ds
    xor ax, ax
    mov es, ax
    assume es:nothing
		lds	ax, dword ptr cs:saved_int1_ptr.ofs
    mov es:(1*sizeof ptr16+ptr16.ofs), ax
    mov word ptr es:(1*sizeof ptr16+ptr16.segm), ds
    pop ds
    pop es

    ; restore 5 interrupts
    assume es:nothing
    push  es
    push  ds
    xor ax, ax
    mov es, ax
    assume es:nothing
    mov ax, cs
    mov ds, ax
    cld
		lea	si, saved_5_interrupt_pointers
		mov	di, (97h*sizeof ptr16)	; from Interrupt 151(0x97) 5 pointers saved
					; Interrupt[0x97 = 151]
					; Interrupt[152]
					; Interrupt[153]
					; Interrupt[154]
					; Interrupt[155]
					;
					; --------
		mov	cx, 5*sizeof ptr16		; 5*sizeof(far-pointer)
		rep movsb		; DS:SI	to address ES:DI

    ; was adlib sound used by game - then the F0 interrupt is overwritten?
		lds	si, es:(0F0h*sizeof ptr16)	; ------
					;
					; the game sets	Interrupt 0xF0 for sound stuff
					; (gets	not set	if the game is not started with	F1)
					;
					; 0:0x3C0 = Interrupt[240 == 0xF0]
					;
					; int_0xF0_proc:
					;   jmp	    short loc_D
					; aIfgmAdlib db	'IFGM ADLIB',0
					; loc_D:
					;   push    ds
					;   push    es
					;   push    si
					;   push    di
					;   push    bx
					;   push    cx
					;   push    bp
					;   cld
					;   mov	    bx,	cs
					;   mov	    ds,	bx
					;   xor	    bx,	bx
					;   mov	    bl,	ah
					;   call    cs:off_EFF[bx]
					;   pop	    bp
					;   pop	    cx
					;   pop	    bx
					;   pop	    di
					;   pop	    si
					;   pop	    es
					;   pop	    ds
					;   iret
					;
					;
					; before the exit the loader checks
					; if 'IFGM' is at the 2. byte of the Interrupt-F0-Proc
					;
					; --------
    mov ax, ds
    or  ax, si
		jz	short exit_program
    mov cx, [si+2]
    mov dx, [si+4]
		cmp	cx, 'FI'
		jnz	short exit_program
		cmp	dx, 'MG'
		jnz	short exit_program
    mov ah, 0Eh
		int	0F0h		; Int 0xF0 gets	overwritten by game with ADLIB sound stuff proc

exit_program:				; CODE XREF: start_0+CEj start_0+DAj ...
    pop ds
    pop es
    assume es:nothing
IFDEF __WASM__
just_exit:
ELSE    
just_exit::
ENDIF
    xor al, al
    mov ah, 4Ch
    int 21h   ; DOS - 2+ - QUIT WITH EXIT CODE (EXIT)
start_0   endp ; sp-analysis failed ; AL = exit code

read_config_and_resize_memory proc near ; CODE XREF: start_0+22p
    push  ds
    mov ax, cs
    mov ds, ax
		
IF 1    
    mov	dx, offset config_tat_filename ; "Config.tat"
    mov ah, 3Dh
    mov al, 0
    int 21h   ; DOS - 2+ - OPEN DISK FILE WITH HANDLE
          ; DS:DX -> ASCIZ filename
          ; AL = access mode
          ; 0 - read
		jb	short loc_816
ELSE
    push offset file_handle 
    push 0 ; read
    push offset config_tat_filename
    call dos_file_open
    add sp,3*2
    cmp ax,0
    jnz loc_816
    
    mov ax,cs:file_handle
ENDIF    
    
IF 1    
    mov bx, ax
		mov	dx, offset config_tat_buffer ; overwrites many of the initialization routines (code)
          ; and some other temporary values
          ; good old dirty memory saving style
		mov	cx, 65535	; read max bytes
    mov ah, 3Fh
    int 21h   ; DOS - 2+ - READ FROM FILE WITH HANDLE
          ; BX = file handle, CX = number of bytes to read
          ; DS:DX -> buffer
		jnb	short loc_817	; --> ax = 233h	= 563 bytes
ELSE
    push offset file_bytes_read
    push offset config_tat_buffer
    push ds
    push 65535 ; read max bytes
    push cs:file_handle
    call dos_file_read
    add sp,5*2
    cmp ax,0
    jnz loc_816
    jmp loc_817
ENDIF    

loc_816:				; CODE XREF: read_config_and_resize_memory+Ej
    mov dx,offset error4
    mov ah,09h
    int 21h
    jmp just_exit  

    pop ds
    retn
; ---------------------------------------------------------------------------

loc_817:				; CODE XREF: read_config_and_resize_memory+1Cj
IF 1
    mov ah, 3Eh
    int 21h   ; DOS - 2+ - CLOSE A FILE WITH HANDLE
ELSE
    push cs:file_handle
    call dos_file_close
    add sp,1*2
    cmp ax,0
    jnz loc_816
    jmp get_offsets
ENDIF     

get_offsets:
          ; BX = file handle
          ;
          ; word_78 = offset config_tat_buffer + *(word*)&config_tat_buffer[0]
          ; word_79 = offset config_tat_buffer + *(word*)&config_tat_buffer[2]
          ; word_79 = offset config_tat_buffer + *(word*)&config_tat_buffer[4]
          ; word_326 = offset config_tat_buffer + *(word*)&config_tat_buffer[6]
          ; word_81 = offset config_tat_buffer + *(word*)&config_tat_buffer[8]
		mov	bx, offset config_tat_buffer ; ----------
					;
					; get the 5 offsets in head of config.tat
					;
					; -----------
    mov ax, cs:[bx] ; 0x0E
		add	ax, offset config_tat_buffer
		mov	cs:config_tat_gfx_table_offset,	ax

    ; only the config_tat_gfx_table_offset is relevant - all other offsets are not needed

    ;===================== reduce com program memory usage to minimum

    ; calc paragraphs for complete program
		mov	bx,offset program_end
    shr bx, 1   ; shr bx,4 => bx / 16
    shr bx, 1
    shr bx, 1
    shr bx, 1
    inc bx    ; bx = (bx / 16) + 1
    
    ; adjust memory block of this program
    ; free as much memory as possible
    mov ah, 4Ah
    int 21h   ; DOS - 2+ - ADJUST MEMORY BLOCK SIZE (SETBLOCK)
          ; ES = segment address of block to change
          ; BX = new size in paragraphs
    
    ;===================== how much memory is now available, maybe the different gfx selection need more memory, vga more then cga?
    
    ; ONLY check free memory - do NOT allocate
    ; bx = dos_get_free_paragraphs();
    mov ah, 48h
    mov bx, 0FFFFh  ; maximum available -> will always fail
    int 21h   ; DOS - 2+ - ALLOCATE MEMORY
          ; BX = number of 16-byte paragraphs desired
          ;
          ; CF gets set -> Error
          ; ax = 8 --> Not Enough Mem
          ; bx = size in paras of the largest block of memory available
          ;      with my DOSBOX = 9C62h paragraphs
          
    ; get free memory type
          
    mov si, 0
    cmp bx, 4000h
    jb  short loc_170
    mov si, 4000h
    cmp bx, 6000h
    jb  short loc_170
    mov si, 8000h
    cmp bx, 8000h
    jb  short loc_170
		mov	si, 0C000h	; <-- we've got way enough memory ; 49152 = 0xC000

    ; ----
    ;
    ; // 0b0011000000000001
    ; //   clear bits 1-11 and 14+15
    ; //   keep 1 and 12+13
    ; some_feature_flags &=	0x3001;
    ; size_type =
    ;   (bx	>= 0x8000) ? 3
    ;   : (bx >= 0x6000) ? 2
    ;   : (bx >= 0x4000) ? 1
    ;   : 0;
    ; // set size_type in 14+15
    ; // set first bit
    ; some_feature_flags |=	((size_type << 14) + 1);
    ;
    ; -------------------

loc_170:        ; CODE XREF: read_config_and_resize_memory+A2j
          ; read_config_and_resize_memory+ABj ...
    or  si, 1   ; -> C001h
		mov	ax, cs:some_feature_flags ; ax = 1
		and	ax, 3001h	; 0b0011000000000001
    or  si, ax
		mov	cs:some_feature_flags, si ; some_feature_flags = C001
    ;==========================================

    pop ds
    retn      ; CF still 0 - No Error
    
read_config_and_resize_memory endp


; =============== S U B R O U T I N E =======================================

interrupt_0x97	proc far		; DATA XREF: set_interrupt_vectors_0x97_and_0x24+2o
    nop
    nop
		nop			; <-- set_interrupt_vectors_0x97_and_0x24 points here (optional+1)
					;
					; this or the next nop
					;
					; leaves some space for	something/aligment???
					;
					;
					; -------
    nop
		mov	ax, cs:some_feature_flags
    iret
interrupt_0x97	endp

; ---------------------------------------------------------------------------

shutdown_speaker proc near
    cli
    in  al, 61h   ; PC/XT PPI port B bits:
					; 0: Tmr 2 gate	ÍËÍ OR	03H=spkr ON
					; 1: Tmr 2 data	Í¼  AND	0fcH=spkr OFF
          ; 3: 1=read high switches
          ; 4: 0=enable RAM parity checking
          ; 5: 0=enable I/O channel check
          ; 6: 0=hold keyboard clock low
          ; 7: 0=enable kbrd
    and al, 0FCh ; 0b11111100
    ; !!!! shutdown speaker
    out 61h, al   ; PC/XT PPI port B bits:
					; 0: Tmr 2 gate	ÍËÍ OR	03H=spkr ON
					; 1: Tmr 2 data	Í¼  AND	0fcH=spkr OFF
          ; 3: 1=read high switches
          ; 4: 0=enable RAM parity checking
          ; 5: 0=enable I/O channel check
          ; 6: 0=hold keyboard clock low
          ; 7: 0=enable kbrd
    sti
    retn
shutdown_speaker endp


some_loading_msg_some_PPI_action_and_back_to_main_menu:	; CODE XREF: start_0+308j
          ; start_0+322j

    call shutdown_speaker

    mov dx,offset error5
    mov ah,09h
    int 21h
    jmp just_exit  

; ---------------------------------------------------------------------------

start_game:       ; CODE XREF: start_0+82j
          ; DATA XREF: seg000:main_menu_jump_tableo
    
    mov bx,gfx_mode

    ; set interrupt_vt[0x9F].offset = gxf_mode - this is how the game knows the to start gfx mode?   
    push  ds
    push  ax
    xor ax, ax
    mov ds, ax
    assume ds:nothing
    mov ax, bx
    mov ds:(9Fh*sizeof ptr16), ax ; Interrupt[9F]
    pop ax
    pop ds

    ; select gfx code offsets in progs.cc1
    assume ds:seg000
    shl bx, 1   ; gfx-index*2
		add	bx, cs:config_tat_gfx_table_offset
    mov bx, cs:[bx]
    or  bx, bx ; offset for this select gfx not null?
		jz	short cancel_game_start
		add	bx, offset config_tat_buffer

loc_173:        ; CODE XREF: start_0+30Dj start_0+34Ej
    push bx
    call  c_START_GAME_sub_11
    add sp,1*2
   
					;
					; very strange construct with START_GAME_sub_11
					; (calling START_GAME_sub_22 itself) and
					; the freestanding START_GAME_sub_22
					; some sort of statemachine?
					;
					; and interrupt	0x97/0x24 gets set severals times the same?
					;
					; POSSIBLE REASON: the game-code was splitted to multiple floppies with	smaller	sized floppies
					; grafixs,sound,music etc.
					;
					; ANOTHER REASON:
					; Intro	code is	splitted from the real game
					;
					; 4 times at this point	before start
					;
					; CF is	set on error
					;
					; ------------
		jb	short cancel_game_start
		mov	al, cs:[bx+gfx_block_t.byte_13h] ; 50h from the	GFX-Block[19]
    and al, 7
		cmp	al, 0		; BLOCK	1?
		jnz	short all_parts_available ; not	taken with 0x50, but with 0x91
					;
					;     0x50=0b01010000
					;     0x91=0b10010001
					; and 0x20=0b00100000
					; and 0x07=0b00000111

    push bx
    call c_GAME_START_sub_7
    add sp,1*2
    
		jb	short some_loading_msg_some_PPI_action_and_back_to_main_menu
		add	bx, size gfx_block_t
    jmp short loc_173
; ---------------------------------------------------------------------------

all_parts_available:			; CODE XREF: start_0+303j start_0+335j
		push	bx		;
					;
					; ----------------
					;
					;
					;
					; this code gets only runned one time
					;
					;
					; -----------------

before_and_after_game_run:		; CODE XREF: start_0+327j
    push bx
    call  c_START_GAME_sub_11
    add sp,1*2
    
		jb	short after_game_run
		mov	al, cs:[bx+gfx_block_t.byte_13h]
    and al, 7
		cmp	al, 1		; BLOCK	2?
		jnz	short after_game_run

    push bx
    call c_GAME_START_sub_7
    add sp,1*2
    
		jb	short some_loading_msg_some_PPI_action_and_back_to_main_menu
		add	bx, size gfx_block_t ; next gfx-block
		jmp	short before_and_after_game_run
; ---------------------------------------------------------------------------

after_game_run:				; CODE XREF: start_0+313j start_0+31Dj
    pop ax
    xchg  ax, bx
    xor cx, cx
    mov es, cx
    assume es:nothing
		cmp	cs:subprogram_exit_code, 0FFh ;	game subprocess	return code?
		jnz	short all_parts_available

    push bx
    call  c_GAME_START_sub_7
    add sp,1*2
    
		jb	short back_to_menu
    mov al, cs:[bx+13h]
    and al, 7
		cmp	al, 2		; BLOCK	3?
		jnz	short cancel_game_start

    push bx
    call  c_GAME_START_sub_7
    add sp,1*2

		jb	short cancel_game_start
    add bx, size gfx_block_t
    jmp short loc_173
; ---------------------------------------------------------------------------

cancel_game_start:			; CODE XREF: start_0+2EBj start_0+2F4j ...
		or	cs:some_feature_flags, 1

back_to_menu:				; CODE XREF: start_0+33Aj
    jmp shutdown_cleanup
    
; END OF FUNCTION CHUNK FOR start_0

; =============== S U B R O U T I N E =======================================

c_START_GAME_sub_11	proc near
  
  ; the interface
  block_ = word ptr 4 

  push bp
  mov bp,sp
  
  PREPARE_BEFORE_CALL
 
  ; set register "parameter"
  mov bx,[bp+block_]
  
  call START_GAME_sub_11

  CLEANUP_AFTER_CALL

  pop bp  
  
  retn
c_START_GAME_sub_11	endp

START_GAME_sub_11 proc near		; CODE XREF: start_0:loc_173p
the_start:
					; start_0:before_and_after_game_runp ...
		cmp	cs:[bx+gfx_block_t.filename], 0FFh ; GFX-Block[GFX][0] == 0xFF? last block
		jz	short is_end_block ; starting with 0xFF
		test	cs:[bx+gfx_block_t.byte_13h], 20h ; -----
					;
					; if((cs:[bx+19] & 0b00100000) == 0)
					; 0x50 & 0x20 =	0
					; 0x91 & 0x20 =	0
					;
					; ----
		jz	short is_useable_block

    push bx
    call  c_GAME_START_sub_7
    add sp,1*2

		jb	short cancel_game_start
		add	bx, size gfx_block_t ; sizeof(GFX-Block) == 18h
		jmp	short the_start
; ---------------------------------------------------------------------------

is_useable_block:			; CODE XREF: START_GAME_sub_11+Bj
    clc
    retn
; ---------------------------------------------------------------------------

is_end_block:				; CODE XREF: START_GAME_sub_11+4j
    stc
    retn
START_GAME_sub_11    endp

interrupt_0x24	proc far		; DATA XREF: set_interrupt_vectors_0x97_and_0x24+12o
    mov al, 3 ; dosbox is always DOS 5
    iret
interrupt_0x24	endp ; sp-analysis failed

set_interrupt_vectors_0x97_and_0x24 proc near ;	CODE XREF: START_GAME_sub_22p
          ; INIT_PART_init_stuff_sub_26+5Ep
    push  cs
    pop ds
		lea	dx, interrupt_0x97
    inc dx
    inc dx
		or	dx, 1		; dx = (offset interrupt_0x97 +	2) | 1;	<== dx = 11B5
					;
					; dx |=	1 means	make it	odd if not already
          
    mov ah, 25h ; set interrupt
    mov al, 97h ; interrupt 0x97
    int 21h   ; DOS - SET INTERRUPT VECTOR
          ; AL = interrupt number
          ; DS:DX = new vector to be used for specified interrupt
          
    mov al, 24h   ; interrupt 0x24
		mov	dx, offset interrupt_0x24
    int 21h   ; DOS - SET INTERRUPT VECTOR
          ; AL = interrupt number
          ; DS:DX = new vector to be used for specified interrupt
          
    retn
set_interrupt_vectors_0x97_and_0x24 endp

SIMPLE_INIT_routine proc near ; CODE XREF: start_0+1Fp
    push  ds

    ; command line    
    cmp byte ptr ds:[80h],2
    jnz command_line_error
    cmp byte ptr ds:[81h],20h
    jnz command_line_error
    cmp byte ptr ds:[82h],'0'
    jb command_line_error
    cmp byte ptr ds:[82h],'4'
    jg command_line_error
    cmp byte ptr ds:[83h],0dh
    jnz command_line_error
    xor ax,ax
    mov al,ds:[82h]
    sub ax,'0'
    mov cs:gfx_mode,ax
    jmp ok
    
command_line_error:
    mov dx,offset cmd_error
    mov ah,09h
    int 21h    
    
    mov ax,4c00h
    int 21h
    
ok:    
    ; save current video mode
    mov ah, 0Fh
    int 10h   ; - VIDEO - GET CURRENT VIDEO MODE
          ; Return: AH = number of columns on screen
          ; AL = current video mode
          ; BH = current active display page
		mov	cs:saved_video_mode, al
    
    ; save interrrupt ptr 1
    push  es
    xor ax, ax
    mov es, ax    ; https://wiki.osdev.org/Memory_Map_(x86)
          ; https://wiki.osdev.org/Interrupt_Vector_Table
          ;
					; ivt[1].ofs(4)/seg(6)
          ;
          ; interrupt 1 is reserved
    assume es:nothing
		mov	ax, es:4	; interrupt 1 ofs
    mov cs:saved_int1_ptr.ofs, ax
		mov	ax, es:6	; interrupt 1 seg
		mov	cs:saved_int1_ptr.segm,	ax
    pop es
    assume es:nothing
    
    ; save 5 interrupt pointers
    push  es
    push  ds
    xor ax, ax
		mov	ds, ax		; ds = 0
    assume ds:nothing
    mov ax, cs
    mov es, ax    ; es = cs
    assume es:seg000
    cld
		lea	di, saved_5_interrupt_pointers
		mov	si, 25Ch	; 0x97 * sizeof(ptr16) = 0x25C -> save 5 interrupt far ptrs
					; starting with	interrupt 0x97 (151)
		mov	cx, 20		; sizeof(ptr16)*5
    rep movsb
    pop ds
    assume ds:seg000
    pop es
    assume es:nothing
   
    call  set_interrupt_vectors_0x97_and_0x24
    
    pop ds
    retn
SIMPLE_INIT_routine endp    

IF 1

extern error9:byte
extern stack_space_end_unk_342:byte
extern config_tat_buffer:byte
extern error4:byte
extern error5:byte
extern gfx_mode:word
extern cmd_error:byte
extern program_end:byte

ELSE
    ; --> from loader_data.asm
    db 1DEh dup(0BBh)    ; also stack space !!!
stack_space_end_unk_342 dw 0 ; DATA XREF: start_0+Ao start_0+31o

  gfx_mode dw 0
  cmd_error db 'you need to select the video card type',0dh,0ah,0dh,0ah
            db 'ae.com 0-4',0dh,0ah
            db '  0=CGA',0dh,0ah
            db '  1=EGA',0dh,0ah
            db '  2=Tandy',0dh,0ah
            db '  3=Hercules',0dh,0ah
            db '  4=VGA',0dh,0ah
            db 0dh,0ah
            db '  example: ae.com 4',0dh,0ah
            db '  for VGA',0dh,0ah,0dh,0ah
            db '$'
            
  error9 db '9','$'
  error6 db '6','$'
  error8 db '8','$'
  error5 db '5','$'
  error4 db '4','$'
  
; here gets the config.tat content stored
config_tat_buffer db 563 dup(0CCh)

; END OF PROGRAM - memory reduce will remove data here
program_end:

ENDIF

seg000    ends


    end start
