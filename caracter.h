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

#ifndef __CTYPE_INCLUIDO
#define __CTYPE_INCLUIDO

extern unsigned short *__eumbuf;
extern short *__praminusculo;
extern short *__pramaiusculo;

#define eumnum(c) (__eumbuf[(c)] & 0x0001U)
#define ealpha(c) (__eumbuf[(c)] & 0x0002U)
#define ecntrl(c) (__eumbuf[(c)] & 0x0004U)
#define eumdigito(c) (__eumbuf[(c)] & 0x0008U)
#define eumgrafico(c) (__eumbuf[(c)] & 0x0010U)
#define eminusculo(c) (__eumbuf[(c)] & 0x0020U)
#define eprint(c) (__eumbuf[(c)] & 0x0040U)
#define epunct(c) (__eumbuf[(c)] & 0x0080U)
#define eespaco(c) (__eumbuf[(c)] & 0x0100U)
#define emaiusculo(c) (__eumbuf[(c)] & 0x0200U)
#define edigitox(c) (__eumbuf[(c)] & 0x0400U)
#define paraminusculo(c) (__praminusculo[(c)])
#define paramaiusculo(c) (__pramaiusculo[(c)])

#endif
