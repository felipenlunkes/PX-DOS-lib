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

#ifndef __STDDEF_INCLUIDO
#define __STDDEF_INCLUIDO

typedef int ptrdiff_t;
#ifndef __TAMANHO_T_DEFINIDO
#define __TAMANHO_T_DEFINIDO

typedef unsigned int tamanho_t;

#endif

#ifndef __WCHAR_T_DEFINIDO
#define __WCHAR_T_DEFINIDO
#ifndef _WCHAR_T_DEFINIDO
#define _WCHAR_T_DEFINIDO
#endif
typedef char wchar_t;
#endif

#define NULL ((void *)0)
#define offsetof(x, y) (tamanho_t)&(((x *)0)->y)

#endif
