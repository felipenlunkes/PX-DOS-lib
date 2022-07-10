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

#ifndef __LIMITES_INCLUIDO
#define __LIMITES_INCLUIDO

#define CHAR_BIT 8
#define SCHAR_MIN -128
#define SCHAR_MAX 127
#define UCHAR_MAX 255
#if ('\x80' < 0)
#define CHAR_MIN -128
#define CHAR_MAX 127
#else
#define CHAR_MIN 0
#define CHAR_MAX 255
#endif
#define MB_LEN_MAX 1
#define SHRT_MIN (-SHRT_MAX-1)
#define SHRT_MAX 32767
#define USHRT_MAX ((unsigned short)65535U)



#define INT_MIN (-INT_MAX-1)
#define INT_MAX 32767
#define UINT_MAX 65535U

#define LONG_MIN (-LONG_MAX-1)
#define LONG_MAX 2147483647L
#define ULONG_MAX 4294967295UL

#endif
