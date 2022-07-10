//******************************************************************
//
// Biblioteca de Funções C ara PX-DOS (LIBPXDOSC)
//
// Versão 1.6.rc2
//
// Compatível com PX-DOS 0.1.1 e 0.9.0+
//
// Copyright (C) 2013-2016 Felipe Miguel Nery Lunkes
//
//******************************************************************

#ifndef __SETJMP_INCLUIDO
#define __SETJMP_INCLUIDO

typedef struct {

    int eax;
    int ebx;
    int ecx;

    int longj;
    int ret;
} jmp_buf[1];

int definirjmp(jmp_buf env);
void longjmp(jmp_buf env, int val);

#endif
