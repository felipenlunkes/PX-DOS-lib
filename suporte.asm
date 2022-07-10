;;/******************************************************************
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

extrn ___divide:proc
extrn ___modulo:proc

public fidrqq
public fiwrqq
public fierqq

_DATA   segment word public 'DATA'
fidrqq  dw  ?
fiwrqq  dw  ?
fierqq  dw  ?
_DATA   ends
_BSS    segment word public 'BSS'
_BSS    ends

DGROUP  group   _DATA,_BSS
        assume cs:_TEXT,ds:DGROUP

_TEXT segment word public 'CODE'

public ___abrir

___abrir proc

push bp
mov bp, sp

push bx
push dx
push ds

mov dx,[bp+8]
mov ds,dx
mov dx,[bp+6]
mov al,[bp+11]

mov ah, 3dh
int 21h

jc ___abrir1

mov dx,[bp+14]
mov ds,dx
mov bx,[bp+12]

mov word ptr [bx], 0

jmp short ___abrir2

___abrir1:

mov dx,[bp+14]
mov ds,dx
mov bx,[bp+12]
mov word ptr [bx], 1

___abrir2:

pop ds
pop dx
pop bx

pop bp
ret
___abrir endp


public ___ler

___ler proc

push bp
mov bp,sp

push bx
push cx
push dx
push ds

mov bx,[bp+6]
mov dx,[bp+10]
mov ds,dx
mov dx,[bp+8]
mov cx,[bp+12]

mov ah, 3fh
int 21h

jc ___ler1

mov dx,[bp+16]
mov ds,dx
mov bx,[bp+14]
mov word ptr [bx], 0

jmp short ___ler2

___ler1:

mov dx,[bp+16]
mov ds,dx
mov bx,[bp+14]
mov word ptr [bx], 1

___ler2:

pop ds
pop dx
pop cx
pop bx

pop bp
ret

___ler endp


public ___escrever

___escrever proc

push bp
mov bp,sp

push bx
push cx
push dx
push ds

mov bx,[bp+6]
mov dx,[bp+10]
mov ds,dx
mov dx,[bp+8]
mov cx,[bp+12]

mov ah, 40h
int 21h

jc ___escrever1

mov dx,[bp+16]
mov ds,dx
mov bx,[bp+14]
mov word ptr [bx], 0

jmp short ___escrever2

___escrever1:

mov dx,[bp+16]
mov ds,dx
mov bx,[bp+14]
mov word ptr [bx], 1

___escrever2:

pop ds
pop dx
pop cx
pop bx

pop bp
ret

___escrever endp



public ___obter

___obter proc

push bp
mov bp, sp
push bx
push dx
push cx

mov bx, [bp + 8]
mov dx, [bp + 10]
mov cx, [bp + 12]
mov al, [bp + 13]

mov ah, 042h
int 021h

pop cx
pop dx
pop bx
pop bp
ret

___obter endp


public ___fechar

___fechar proc

push bp
mov bp, sp
push bx

mov bx,[bp+8]

mov ah, 03eh
int 021h

pop bx
pop bp
ret

___fechar endp


public ___remover

___remover proc

push bp
mov bp, sp
push ds
push dx

mov dx, [bp + 8]
mov ds, [bp + 10]

mov ah, 041h
int 021h

pop dx
pop ds
pop bp
ret

___remover endp


public ___renomear

___renomear proc

push bp
mov bp, sp
push ds
push dx
push es
push di

mov dx, [bp + 8]
mov ds, [bp + 10]
mov di, [bp + 12]
mov es, [bp + 14]

mov ah, 056h
int 021h

pop di
pop es
pop dx
pop ds
pop bp
ret

___renomear endp



public ___alocarmem

___alocarmem proc

push bp
mov bp,sp

push bx
push dx
push ds

mov bx,[bp+6]

shr bx,1
shr bx,1
shr bx,1
shr bx,1

add bx,1
mov ah, 48h
int 21h

jnc allocok

mov ax, 0

allocok:

mov dx,[bp+10]
mov ds,dx
mov bx,[bp+8]

mov word ptr [bx], 0
mov word ptr [bx+2], ax

pop ds
pop dx
pop bx
pop bp
ret

___alocarmem endp


public ___memorialivre

___memorialivre proc

push bp
mov bp,sp
push es
push dx
push cx

mov dx,[bp+6]
mov cx,[bp+8]

shr dx, 1
shr dx, 1
shr dx, 1
shr dx, 1

add cx, dx
mov es, cx
mov ah, 049h
int 21h

pop cx
pop dx
pop es
pop bp
ret

___memorialivre endp


public ___exec

___exec proc

push bp
mov bp, sp
push ds
push dx
push es
push bx
push cx
push si
push di

mov dx, [bp + 6]
mov ds, [bp + 8]
mov bx, [bp + 10]
mov es, [bp + 12]

jmp short passoapasso

salvar_sp dw ?
salvar_ss dw ?
passoapasso:

mov salvar_sp, sp
mov salvar_ss, ss

mov al, 0
mov ah, 04bh
cld              ; dos 2 "bug"
int 21h

cli
mov ss, salvar_ss
mov sp, salvar_sp
sti

pop di
pop si
pop cx
pop bx
pop es
pop dx
pop ds
pop bp
ret

___exec endp


public ___datahora

___datahora proc

push bp
mov bp, sp
push ds
push dx
push cx
push bx

mov bx, [bp + 6]
mov ds, [bp + 8]

mov ah, 02ah
int 021h

mov [bx + 0], cx
mov ch, 0
mov cl, dh
mov [bx + 2], cx
mov dh, 0
mov [bx + 4], dx

mov ah, 02ch
int 021h

mov ah, 0
mov al, ch
mov [bx + 6], ax
mov ch, 0
mov [bx + 8], cx
mov ah, 0
mov al, dh
mov [bx + 10], ax
mov dh, 0
mov [bx + 12], dx

pop bx
pop cx
pop dx
pop ds
pop bp
ret

___datahora endp


ifdef WATCOM

public __U4D

__U4D proc

push cx
push bx
push dx
push ax
push cx
push bx
push dx
push ax

call far ptr f_lumod@

mov cx, dx
mov bx, ax

call far ptr f_ludiv@

ret

__U4D endp
endif


public f_ludiv@

f_ludiv@ proc far

push bp
mov bp,sp
push bx

cmp word ptr [bp + 12], 0
jne ludiv_full

mov ax, [bp + 8]
mov dx, 0
div word ptr [bp + 10]
mov bx, ax
mov ax, [bp + 6]
div word ptr [bp + 10]

mov dx, bx
jmp short ludiv_fin

ludiv_full:

push word ptr [bp + 12]
push word ptr [bp + 10]
push word ptr [bp + 8]
push word ptr [bp + 6]

call far ptr ___divide

add sp, 8 

ludiv_fin:

pop bx
pop bp
ret 8
f_ludiv@ endp




ifdef WATCOM

public __I4D

__I4D proc

push cx
push bx
push dx
push ax
push cx
push bx
push dx
push ax

call far ptr f_lmod@

mov cx, dx
mov bx, ax

call far ptr f_ldiv@

ret

__I4D endp
endif



public f_ldiv@

f_ldiv@ proc

push bp
mov bp,sp
push dx

cmp word ptr [bp + 12], 0
jne ldiv_full

mov ax,[bp+6]
mov dx,[bp+8]

idiv word ptr [bp+10]

jmp short ldiv_fin

ldiv_full:

push word ptr [bp + 12]
push word ptr [bp + 10]
push word ptr [bp + 8]
push word ptr [bp + 6]

call far ptr ___divide
add sp, 8 

ldiv_fin:

pop dx
pop bp
ret 8

f_ldiv@ endp


public f_lmod@

f_lmod@ proc

push bp
mov bp,sp

cmp word ptr [bp + 12], 0
jne lmod_full

mov ax,[bp+6]
mov dx,[bp+8]
idiv word ptr [bp+10]
mov ax,dx
mov dx,0

jmp short lmod_fin

lmod_full:

push word ptr [bp + 12]
push word ptr [bp + 10]
push word ptr [bp + 8]
push word ptr [bp + 6]

call far ptr ___modulo

add sp, 8 

lmod_fin:

pop bp
ret 8

f_lmod@ endp



public f_lumod@

f_lumod@ proc

push bp
mov bp,sp

cmp word ptr [bp + 12], 0
jne lumod_full

mov ax, [bp + 8]
mov dx, 0
div word ptr [bp + 10]
mov ax, [bp + 6]
div word ptr [bp + 10]
mov ax,dx
mov dx, 0

jmp short lumod_fin

lumod_full:

push word ptr [bp + 12]
push word ptr [bp + 10]
push word ptr [bp + 8]
push word ptr [bp + 6]

call far ptr ___modulo

add sp, 8 

lumod_fin:

pop bp
ret 8

f_lumod@ endp



public __I4M

__I4M:

public __U4M

__U4M:

public f_lxmul@

f_lxmul@ proc

push bp
mov bp,sp
push cx

push ax
mul cx
mov cx, ax
pop ax
mul bx
add dx, cx

pop cx
pop bp
ret

f_lxmul@ endp



public f_lxlsh@

f_lxlsh@ proc

push bx

cmp cl, 24
jl lxlsh_16
mov dh, al
mov dl, 0
mov ax, 0
sub cl, 24

jmp short lxlsh_last

lxlsh_16:

cmp cl, 16
jl lxlsh_8

mov dx, ax
mov ax, 0
sub cl, 16

jmp short lxlsh_last

lxlsh_8:

cmp cl, 8

jl lxlsh_last
mov dh, dl
mov dl, ah
mov ah, al
mov al, 0
sub cl, 8

;jmp short lxlsh_last

lxlsh_last:

mov ch, 8
sub ch, cl
xchg ch, cl
mov bx, ax
shr bx, cl
xchg ch, cl
shl dx, cl
or dl, bh
shl ax, cl

pop bx
ret

f_lxlsh@ endp




public f_lxursh@

f_lxursh@ proc

push bx

cmp cl, 24

jl lxursh_16

mov al, dh
mov ah, 0
mov dx, 0
sub cl, 24

jmp short lxursh_last

lxursh_16:

cmp cl, 16

jl lxursh_8

mov ax, dx
mov dx, 0
sub cl, 16

jmp short lxursh_last

lxursh_8:

cmp cl, 8

jl lxursh_last

mov al, ah
mov ah, dl
mov dl, dh
mov dh, 0
sub cl, 8
;jmp short lxursh_last

lxursh_last:

mov ch, 8
sub ch, cl
xchg ch, cl
mov bx, dx
shl bx, cl
xchg ch, cl
shr ax, cl
or ah, bl
shr dx, cl

pop bx
ret

f_lxursh@ endp


public f_scopy@

f_scopy@ proc

push bp
mov bp, sp
push cx
push ds
push es
push si
push di
lds si, [bp + 6]
les di, [bp + 10]
cld
rep movsb

pop di
pop si
pop es
pop ds
pop cx
pop bp
ret 8

f_scopy@ endp


public f_ftol@

f_ftol@ proc

ret

f_ftol@ endp

public ___SIN__

___SIN__ proc

ret

___SIN__ endp

public ___COS__

___COS__ proc

ret

___COS__ endp

ifdef WATCOM

public __CHP

__CHP proc

ret

__CHP endp

endif

_TEXT ends

end
