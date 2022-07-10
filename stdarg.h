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

#ifndef __STDARG_INCLUIDO
#define __STDARG_INCLUIDO

typedef char * va_list;

#define va_start(ap, parmN) ap = (char *)&parmN + 4
#define va_arg(ap, type) *(type *)(ap += sizeof(type), ap - sizeof(type))
#define va_end(ap) ap = 0

#endif
