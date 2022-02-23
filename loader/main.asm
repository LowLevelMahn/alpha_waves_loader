.286
.model tiny

seg000 segment byte public 'CODE'

assume cs:seg000
org 100h
assume es:nothing, ss:nothing, ds:seg000

extern start_0:near

start proc near
  jmp start_0
start endp

seg000 ends

end start
  