;;******************************************************************
;;
;; Biblioteca de Funções C ara PX-DOS (LIBPXDOSC)
;;
;; Versão 1.6.rc2
;;
;; Compatível com PX-DOS 0.1.1 e 0.9.0+
;;
;; Copyright (C) 2013-2016 Felipe Miguel Nery Lunkes
;;
;;******************************************************************

.model large

extrn ___iniciar:proc

public ___psp
public ___ponteirodeambiente
public ___verso

_DATA   segment word public 'DATA'
banner  db  "Biblioteca C para PX-DOS (LIBPXDOSC) - Copyright (C) 2013-2016 Felipe Miguel Nery Lunkes"
___psp   dd  ?
___ponteirodeambiente dd ?
___verso dw ?
_DATA   ends
_BSS    segment word public 'BSS'
_BSS    ends
_STACK  segment word stack 'STACK'
        db 1000h dup(?)
_STACK  ends

DGROUP  group   _DATA,_BSS
        assume cs:_TEXT,ds:DGROUP

_TEXT segment word public 'CODE'

top:

___inicioInt proc



nop
nop
nop
nop

push ds
mov ax, 0
push ax



mov ax, sp
mov cl, 4
shr ax, cl 
mov bx, ss
add ax, bx
add ax, 2 
mov bx, es
sub ax, bx 



mov bx, ax
mov ah, 4ah
int 21h

mov dx,DGROUP
mov ds,dx

mov ah,30h
int 21h
xchg al,ah
mov [___verso],ax

mov word ptr ___psp, 0
mov word ptr [___psp + 2], es
mov word ptr ___ponteirodeambiente, 0
mov dx, es:[02ch]
mov word ptr [___ponteirodeambiente + 2], dx
mov dx, ds
mov es, dx


call far ptr ___iniciar
add sp, 4  

push ax


call far ptr ___saira
add sp, 2
ret
___inicioInt endp

public ___saira
___saira proc
push bp
mov bp, sp
mov ax, [bp + 6]
mov ah,4ch
int 21h 
pop bp
ret
___saira endp


public ___main
___main proc
ret
___main endp


_TEXT ends
          
end top
