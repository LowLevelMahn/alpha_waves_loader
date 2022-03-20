    .286
    .model tiny

; ===========================================================================

; Segment type: Pure code
seg000    segment byte public 'DATA'
    assume cs:seg000
    assume es:nothing, ss:nothing, ds:seg000

public error9
public stack_space_end_unk_342
public config_tat_buffer
public error4
public error5
public gfx_mode
public cmd_error

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

seg000    ends

end