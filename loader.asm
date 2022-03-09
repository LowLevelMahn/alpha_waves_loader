; ---------------------------------------------------------------------------

ptr16		struc ;	(sizeof=0x4)	; XREF:	seg000:maybe_exe_bufferr
					; seg000:some_game_ptrr ...
ofs		dw ?
segm		dw ?
ptr16   ends

; ---------------------------------------------------------------------------

gfx_block_t	struc ;	(sizeof=0x18)
filename	db 18 dup(?)		; XREF:	start_0+30Ao start_0+324o ...
byte_12h	db ?
byte_13h	db ?			; XREF:	GAME_START_sub_7+1Dr
					; start_0+2FBr	...
byte_14h	db ?			; XREF:	START_GAME_IS_GFX_SUPPORTED_sub_12+5r
byte_15h	db ?
word_16h	dw ?			; XREF:	START_GAME_FEATURE_FLAG_STUFF_sub_21r
gfx_block_t	ends

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

word_38		dw 0			; DATA XREF: GAME_START_sub_7+79w
					; GAME_START_sub_7+8Do
    db 80h, 0
word_597	dw 0			; DATA XREF: GAME_START_sub_7+7Ew
    db 5Ch, 0
word_598	dw 0			; DATA XREF: GAME_START_sub_7+83w
    db 6Ch, 0
word_599	dw 0			; DATA XREF: GAME_START_sub_7+88w
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
word_50		dw 0			; DATA XREF: GAME_START_sub_7+96w
					; GAME_START_sub_7+A9r
word_51		dw 0			; DATA XREF: GAME_START_sub_7+91w
					; GAME_START_sub_7+A4r
maybe_exe_header db 28h	dup(0)		; DATA XREF: GAME_START_sub_6+5Do
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
        
byte_57		db 0			; DATA XREF: GAME_START_sub_3+42o
					; GAME_START_sub_3+7Cr	...
byte_569	db 0			; DATA XREF: GAME_START_sub_3:loc_577r
word_60		dw 0			; DATA XREF: GAME_START_sub_3+76r
					; GAME_START_sub_3+EDr	...
    db 4 dup(0)
another_pointer2 ptr16 <0>		; DATA XREF: GAME_START_sub_3+49r
					; GAME_START_sub_3+61w	...
word_62		dw 0			; DATA XREF: EXE_HEADER_sub_2+17w
					; EXE_HEADER_sub_2+9Ar	...
word_63		dw 0			; DATA XREF: EXE_HEADER_sub_2+1Cw
					; EXE_HEADER_sub_2+95r	...
dta_seg		dw 0			; DATA XREF: EXE_HEADER_sub_2+2Dr
					; EXE_HEADER_sub_2+103r ...
word_558	dw 0			; DATA XREF: EXE_HEADER_sub_2:loc_557w
word_559	dw 0			; DATA XREF: EXE_HEADER_sub_2+12w
word_68		dw 0			; DATA XREF: EXE_HEADER_sub_2+E3w
					; EXE_HEADER_sub_2+10Br
word_69		dw 0			; DATA XREF: EXE_HEADER_sub_2+EDw
					; EXE_HEADER_sub_2+10Fr
maybe_game_code_ptr ptr16 <0>		; DATA XREF: EXE_HEADER_sub_2+FFw
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
          
config_tat_size	dw 0			; DATA XREF: read_config_and_resize_memory:loc_817w

some_feature_flags dw 1			; DATA XREF: read_config_and_resize_memory+BCr
					; read_config_and_resize_memory+C5w ...
					;
					;
					;
					; --------------------------
					; bit[	0] = ??? [active on startup]
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
					; bit[ 13] = currency =	franc
					; bit[14+15]  =	mem size type?
					;	     = 00 = 0 (< 0x4000	free paragraphs)
					;	     = 01 = 1 (>= 0x4000 free paragraphs)
					;	     = 10 = 2 (>= 0x6000 free paragraphs)
					;	     = 11 = 3 (>= 0x8000 free paragraphs)
					; -------------------------

subprogram_exit_code db	0		; DATA XREF: GAME_START_sub_7+B3w

saved_video_mode db 0			; DATA XREF: start_0+89r
    
progs_cc1_filename db 'PROGS.CC1',0   
    
; =============== S U B R O U T I N E =======================================


; void __usercall GAME_START_sub_1(__int16 unknown_@<si>)
GAME_START_sub_1 proc near		; CODE XREF: EXE_HEADER_sub_2+8Ep
					; EXE_HEADER_sub_2+B2p
    push  ds
    push  es
    push  si
    push  di
    push  bx
    push  cx
    cld

loc_556:				; CODE XREF: GAME_START_sub_1+47j
    mov ax, si
    shr ax, 1
    shr ax, 1
    shr ax, 1
    shr ax, 1
    mov dx, ds
    add dx, ax
    mov ds, dx
    and si, 0Fh
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

loc_555:				; CODE XREF: GAME_START_sub_1+35j
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
GAME_START_sub_1 endp


; =============== S U B R O U T I N E =======================================


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
    mov cs:word_558, ax
    mov cs:word_559, dx
    mov cs:word_62, cx
    mov cs:word_63, bx
		mov	cs:exe_pointer.ofs, si
		mov	cs:exe_pointer.segm, ds
    mov ah, 50h
		mov	bx, cs:dta_seg
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
    pop ds
    pop ax
    pop es
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
		cmp	word ptr [si], 'ZM' ; 'MZ' or 'ZM' exe header signature??
    jz  short loc_562
    stc
    pop bx
		retn			; near ret from	far proc - manually fixed stack?
; ---------------------------------------------------------------------------

loc_562:				; CODE XREF: EXE_HEADER_sub_2+6Bj
		mov	bx, [si+8]	; exe Header size
    xor ax, ax
		shl	bx, 1		; bx *=	2;
    rcl ax, 1
		shl	bx, 1		; bx *=	2;
    rcl ax, 1
		shl	bx, 1		; bx *=	2;
    rcl ax, 1
		shl	bx, 1		; bx *=	2;
		rcl	ax, 1		;
					;
					; ==> bx *= 16
					; ==> ax contains the hi word
					;
					; results in a 32bit numbers in	ax:bx
					;
					; uint32_t result = header_paragraphs *	16;
					; ax ==	(result	>> 16));
					; bx ==	(result	& 0xFFFF));
		les	di, dword ptr cs:pointer3.ofs
    mov cx, bx
    mov bx, ax
		call	GAME_START_sub_1
    mov dx, bx
    mov si, cx    ; unknown_@
    mov bx, cs:word_63
    mov cx, cs:word_62
    sub cx, si
    sbb bx, dx
    mov cs:word_63, bx
    mov cs:word_62, cx
		les	di, dword ptr cs:exe_pointer.ofs
		call	GAME_START_sub_1
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
    mov ax, es:[si+10h]
    mov cs:word_68, ax
    mov ax, es:[si+0Eh]
    add ax, bp
    mov cs:word_69, ax
    mov ax, es:[si+16h]
    add ax, bp
		mov	cs:maybe_game_code_ptr.segm, ax
    mov ax, es:[si+14h]
		mov	cs:maybe_game_code_ptr.ofs, ax
		mov	es, cs:dta_seg
    cld
    mov di, es
    mov ax, cs:word_68
    mov dx, cs:word_69
    mov cx, cs:word_62
    mov bx, cs:word_63
    cli
    mov ss, dx
    mov sp, ax
    mov ds, di
    xor ax, ax
    push  ax
    sti
		jmp	dword ptr cs:maybe_game_code_ptr.ofs ; jump into game code?
    ;its a jump - no return 
; ---------------------------------------------------------------------------

loc_561:				; CODE XREF: EXE_HEADER_sub_2+64j
    
    cli
		
    mov	ax, cs:dta_seg
    mov es, ax
    mov ds, ax
    mov ss, ax
    mov sp, 0FFFFh
    xor bx, bx
    push  bx
    push  ax
    mov ax, 100h
    push  ax ; does that creates a correct retf at the end?
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


GAME_START_sub_3 proc near		; CODE XREF: GAME_START_sub_3+101j
          ; read_some_file_sub_4+1EDp
    push  es
    push  di
    mov cx, 80h
    mov ax, ds
    mov es, ax
    assume es:seg000
		mov	di, 301h	; its not in seg000!!!
    xor ax, ax
    rep stosw
    pop di
    pop es
    assume es:nothing
    push  es
    xor ax, ax
    mov es, ax
    assume es:nothing
    sub di, es:4
    pop es
    assume es:nothing
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
    add di, es:4
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
    mov ax, si
    shr ax, 1
    shr ax, 1
    shr ax, 1
    shr ax, 1
    mov dx, ds
    add ax, dx
    mov ds, ax
    and si, 0Fh
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
    pop di
    pop es
    assume es:nothing
    pop ds
    xor ch, ch
    mov cl, cs:byte_57
    xor ah, ah
    mov bx, 1

loc_567:				; CODE XREF: GAME_START_sub_3+EBj
    mov al, [bx+200h]
    mov si, ax
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
		jmp	GAME_START_sub_3
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


read_some_file_sub_4 proc near		; CODE XREF: GAME_START_sub_7+2Ap
    ; bx = offset 0 of gfx block == filename == always progs.cc1
    mov ax, cs
    mov ds, ax

IF 1		
    ; its always "progs.cc1"
    mov dx, offset progs_cc1_filename
ELSE    
    mov	dx, bx		; bx = offset filename
ENDIF    
    
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
    mov si, bx
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
    mov al, cs:[si+12h]
    xor ah, ah
    shl ax, 1
    shl ax, 1
    mov dx, ax
    xor cx, cx
    mov al, 1
    mov ah, 42h
    int 21h   ; DOS - 2+ - MOVE FILE READ/WRITE POINTER (LSEEK)
          ; AL = method: offset from present location
    jb  short loc_136
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
    shr di, 1
    rcr si, 1
    shr di, 1
    rcr si, 1
    shr di, 1
    rcr si, 1
    shr di, 1
    rcr si, 1
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
    mov cx, dx
    shr cx, 1
    shr cx, 1
    shr cx, 1
    shr cx, 1
    mov ax, ds
    add ax, cx
    mov ds, ax
    and dx, 0Fh
    mov ax, 0BB80h
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
		les	di, dword ptr cs:maybe_exe_buffer.ofs
    push  es
    xor ax, ax
    mov es, ax
    assume es:nothing
    mov es:4, di
    pop es
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
    shr cx, 1
    rcr si, 1
    shr cx, 1
    rcr si, 1
    shr cx, 1
    rcr si, 1
    shr cx, 1
    rcr si, 1
    mov ax, es
    add ax, si
		mov	cs:maybe_exe_buffer.segm, ax
    inc ax
    mov ds, ax
    and bx, 0Fh
		mov	cs:maybe_exe_buffer.ofs, bx
    cld
		call	GAME_START_sub_3
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
    shr bx, 1
    rcr ax, 1
    shr bx, 1
    rcr ax, 1
    shr bx, 1
    rcr ax, 1
    shr bx, 1
    rcr ax, 1
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
    shr ax, 1
    rcr bx, 1
    shr ax, 1
    rcr bx, 1
    shr ax, 1
    rcr bx, 1
    shr ax, 1
    rcr bx, 1
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


GAME_START_sub_6 proc near		; CODE XREF: GAME_START_sub_7+25p
    push  bx
    xor ax, ax
		mov	cs:far_ptr3.ofs, ax
		mov	cs:far_ptr3.segm, ax
    mov al, cs:byte_55
    test  al, 20h
    jnz short loc_592
		mov	cs:far_ptr3.ofs, 100h
		mov	dx, cs:maybe_exe_buffer.segm
		add	cs:maybe_exe_buffer.segm, 16
		mov	cs:dta_seg, dx
    mov ah, 26h
    int 21h   ; DOS - CREATE PSP
          ; DX = segment number at which to set up PSP
		mov	dx, 128		; dta_offs
		mov	ds, cs:dta_seg
    mov ah, 1Ah
    int 21h   ; DOS - SET DISK TRANSFER AREA ADDRESS
          ; DS:DX -> disk transfer buffer
    mov word ptr ds:0Ch, cs
		mov	word ptr ds:0Ah, offset	loc_600	; TODO:	could be only a	overwrite buffer - or the real code that resists there
    clc
    pop bx
    retn
; ---------------------------------------------------------------------------

loc_592:				; CODE XREF: GAME_START_sub_6+11j
    xor ax, ax
    mov ds, ax
    assume ds:nothing
    lea ax, ds:169h
    mov ds:26Ch, ax
    mov word ptr ds:26Eh, cs
    mov cx, 0Ah
    mov ax, cs
    mov ds, ax
    assume ds:seg000
		lea	si, maybe_exe_header

loc_594:				; CODE XREF: GAME_START_sub_6+6Bj
    mov ax, [si]
    or  ax, [si+2]
    jz  short loc_593
    add si, 4
    loop  loc_594
    stc
    pop bx
    retn
; ---------------------------------------------------------------------------

loc_593:				; CODE XREF: GAME_START_sub_6+66j
		mov	ax, cs:maybe_exe_buffer.segm
    mov [si+2], ax
    mov word ptr [si], 0
    clc
    pop bx
    retn
GAME_START_sub_6 endp


; =============== S U B R O U T I N E =======================================


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
    pop bx
		mov	al, cs:[bx+gfx_block_t.byte_13h]
    mov cs:byte_55, al
		call	GAME_START_sub_6
    jb  short loc_595
    call  read_some_file_sub_4
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
    shr di, 1
    rcr si, 1
    shr di, 1
    rcr si, 1
    shr di, 1
    rcr si, 1
    shr di, 1
    rcr si, 1
    inc si
    add ax, si
    mov es, ax
    xor di, di
		lds	cx, dword ptr cs:some_game_ptr.ofs
    mov bx, ds
		mov	ax, cs:somway_exe_buffer_seg
    add  ax, 10h
    mov ds, ax
    xor si, si
    mov dx, cs
    mov cs:word_38, dx
    mov cs:word_597, dx
    mov cs:word_598, dx
    mov cs:word_599, dx
    lea ax, word_38
    mov cs:word_51, sp
    mov cs:word_50, ss
		call	near ptr EXE_HEADER_sub_2 ; !!!!calls the game code
    jnb short loc_600
    stc
    pop bx
    retn
; ---------------------------------------------------------------------------

loc_600:
					; DATA XREF: GAME_START_sub_6+3Eo
    cli
    mov sp, cs:word_51
    mov ss, cs:word_50
    sti
    mov ah, 4Dh
    int 21h   ; DOS - 2+ - GET EXIT CODE OF SUBPROGRAM (WAIT)
		mov	cs:subprogram_exit_code, al
		call	GAME_START_sub_5
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
    
    ; prepare, registers and stack
    cld
    cli
    mov ax, cs
    mov ss, ax
    mov ds, ax
    mov es, ax
    mov sp, offset stack_space_end_unk_342
    sti
    
;====== ??? free game memory for restart?
		lea	ax, maybe_exe_header
		mov	cx, 10
    mov ax, cs
    mov ds, ax
		lea	si, maybe_exe_header

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
    add si, 4
		loop	loc_844
		lea	di, maybe_exe_header
    mov ax, cs
    mov es, ax
    assume es:seg000
    xor ax, ax
		mov	cx, 28h
    rep stosw
;======    

    jmp start_game
; ---------------------------------------------------------------------------

shutdown_cleanup:

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
    mov es:4, ax
    mov word ptr es:6, ds
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
		mov	di, 25Ch	; from Interrupt 151(0x97) 5 pointers saved
					;
					; Interrupt[151]
					; Interrupt[152]
					; Interrupt[153]
					; Interrupt[154]
					; Interrupt[155]
					;
					; --------
		mov	cx, 20		; 5*sizeof(far-pointer)
		rep movsb		; DS:SI	to address ES:DI

    ; check if adlib was used by game - interrupt F0 is then set - with adlib signature
		lds	si, es:3C0h	; ------
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
just_exit:
    xor al, al
    mov ah, 4Ch
    int 21h   ; DOS - 2+ - QUIT WITH EXIT CODE (EXIT)
start_0   endp ; sp-analysis failed ; AL = exit code

read_config_and_resize_memory proc near ; CODE XREF: start_0+22p
    push  ds
    mov ax, cs
    mov ds, ax
		mov	dx, offset config_tat_filename ; "Config.tat"
    mov ah, 3Dh
    mov al, 0
    int 21h   ; DOS - 2+ - OPEN DISK FILE WITH HANDLE
          ; DS:DX -> ASCIZ filename
          ; AL = access mode
          ; 0 - read
		jb	short loc_816
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

loc_816:				; CODE XREF: read_config_and_resize_memory+Ej
    mov dx,offset error4
    mov ah,09h
    int 21h
    jmp just_exit  

    pop ds
    retn
; ---------------------------------------------------------------------------

loc_817:				; CODE XREF: read_config_and_resize_memory+1Cj
    mov ah, 3Eh
    int 21h   ; DOS - 2+ - CLOSE A FILE WITH HANDLE
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
		mov	bx,offset config_tat_buffer + size config_tat_buffer
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
					;
					; -------------------
					;
					; bx = allocated dos paragraphs
					; si = 0;
					; if(bx	< 0x4000) goto loc_170;
					; // >=	0x4000
					; si = 0x4000;	  // 0b0100000000000000
					; if(bx	< 0x6000) goto loc_170;
					; // >=	0x6000
					; si = 0x8000;	  // 0b1000000000000000
					; if(bx	< 0x8000) goto loc_170;
					; // >=	0x8000
					; si = 0xC000;	  // 0b1100000000000000
					;
					; loc_170:
					;   si |= 1;	  // 0bxx00000000000001
					;   ax = some_feature_flags;
					;   ax &= 0x3001; // 0b0011000000000001	// clear bits 1-11 and 14+15
					;   si |= ax; // get current flags into	si
					;   some_feature_flags = si; //	set flags
					;
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
    mov ds:27Ch, ax ; 27Ch / 4 => interrupt_vt[0x9F]
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
		call	START_GAME_sub_11 ; ------------
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
		call	START_GAME_sub_22
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
		call	START_GAME_sub_11 ; come through before	game start and after game ends
		jb	short after_game_run
		mov	al, cs:[bx+gfx_block_t.byte_13h]
    and al, 7
		cmp	al, 1		; BLOCK	2?
		jnz	short after_game_run
		call	START_GAME_sub_22 ; !!!!! starts the game (after some iterations) - multipe sub_22 calls before
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
		call	START_GAME_sub_11
		jb	short back_to_menu
    mov al, cs:[bx+13h]
    and al, 7
		cmp	al, 2		; BLOCK	3?
		jnz	short cancel_game_start
		call	START_GAME_sub_22
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


START_GAME_sub_11 proc near		; CODE XREF: start_0:loc_173p
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
		call	START_GAME_sub_22
		jb	short cancel_game_start
		add	bx, size gfx_block_t ; sizeof(GFX-Block) == 18h
		jmp	short START_GAME_sub_11
; ---------------------------------------------------------------------------

is_useable_block:			; CODE XREF: START_GAME_sub_11+Bj
    clc
    retn
; ---------------------------------------------------------------------------

is_end_block:				; CODE XREF: START_GAME_sub_11+4j
    stc
    retn
START_GAME_sub_11    endp

; --------------------
;
; gets called 4	times
;
;
; checks if prog1.cc exists
; AND sets 0 at end of progs1.cc filename
;
; --------------------

START_GAME_DOES_FILE_EXIST_sub_19 proc near ; CODE XREF: START_GAME_sub_22+8p
		mov	si, bx		; bx = offset gfx_block.filename

find_first_blank_loop:			; CODE XREF: START_GAME_DOES_FILE_EXIST_sub_19+9j
		lodsb			; load first char of filename, ++si
    or  al, al
		jz	short found_null_in_name
    cmp al, 20h
		jnz	short find_first_blank_loop
		mov	byte ptr [si-1], 0 ; set filename asciiz 0 at first found 0x20 (blank)

found_null_in_name:
    cld ; always return file exists
    retn  
START_GAME_DOES_FILE_EXIST_sub_19 endp

; =============== S U B R O U T I N E =======================================

START_GAME_FEATURE_FLAG_STUFF_sub_21 proc near ; CODE XREF: START_GAME_sub_22+3p

  	mov	si, cs:[bx+gfx_block_t.word_16h]
		mov	ax, si		; always 0
		and	ax, 0F000h	; 0xF000 = 0b1111000000000000
		mov	cx, cs:some_feature_flags
		and	cx, 0F000h	; 0xF000 = 0b1111000000000000
		and	si, 3		    ; 0x0003 = 0b0000000000000011

    cmp si, 0
    jz loc_206
    cmp si, 1
    jz loc_203
    cmp si, 2
    jz loc_205
    ; or 3
    jmp loc_205

loc_203:				; CODE XREF: START_GAME_FEATURE_FLAG_STUFF_sub_21+17j
          ; DATA XREF: seg000:154Co
    cmp ax, cx
    ja  short loc_204
    stc
    retn
; ---------------------------------------------------------------------------

loc_205:				; CODE XREF: START_GAME_FEATURE_FLAG_STUFF_sub_21+17j
          ; DATA XREF: seg000:154Eo ...
    cmp ax, cx
    ja  short loc_204
    stc
    retn
; ---------------------------------------------------------------------------

loc_204:				; CODE XREF: START_GAME_FEATURE_FLAG_STUFF_sub_21-31j
					; START_GAME_FEATURE_FLAG_STUFF_sub_21-2Bj
    clc
    retn
; ---------------------------------------------------------------------------

   ; TODO: analyse again - error reporting
loc_206:				; CODE XREF: START_GAME_FEATURE_FLAG_STUFF_sub_21+17j
					; DATA XREF: seg000:jump_table_off_653o
    cmp ax, cx
    jb  short loc_651

    call shutdown_speaker

    ; just exit
    mov dx,offset error6
    mov ah,09h
    int 21h
    jmp just_exit 
    
loc_651:
    stc
    retn    

START_GAME_FEATURE_FLAG_STUFF_sub_21 endp

START_GAME_sub_22 proc near		; CODE XREF: start_0+305p start_0+31Fp ...

		call	START_GAME_FEATURE_FLAG_STUFF_sub_21
		jnb	short locret_654 ; if(!CF)...

    jmp loc_655

locret_654:				; CODE XREF: START_GAME_sub_22+6j
    retn
; ---------------------------------------------------------------------------

loc_655:				; CODE XREF: START_GAME_sub_22+Bj
		call	GAME_START_sub_7 ; starts the game code
					; gets also called several times
    retn
START_GAME_sub_22 endp

interrupt_0x24	proc far		; DATA XREF: set_interrupt_vectors_0x97_and_0x24+12o
IF 1
    mov al, 3 ; dosbox is always DOS 5
    iret
ELSE
    cmp cs:dos_version, 3
		jb	short below_DOS_3 ; < DOS 3?
    mov al, 3
    iret
; ---------------------------------------------------------------------------

below_DOS_3:				; CODE XREF: interrupt_0x24+6j
    pop ax
    pop ax
    pop ax
    pop ax
    pop bx
    pop cx
    pop dx
    pop si
    pop di
    mov bp, sp
    or  word ptr [bp+0Ah], 1
    pop bp
    pop ds
    pop es
    assume es:nothing
    iret
ENDIF    
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
    mov ax, 2597h ; al = interrupt 0x97
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
            
  error6 db '6','$'
  error8 db '8','$'
  error5 db '5','$'
  error4 db '4','$'

; here gets the config.tat content stored
config_tat_buffer db 563 dup(0CCh)

seg000    ends


    end start
