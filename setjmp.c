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

#include "setjmp.h"



int definirjmp(jmp_buf env)
{

    env[0].eax = 0;
    env[0].ebx = 0;
    env[0].ecx = 0;
    env[0].longj = 0;
    /* env[0].sp = sp */

    if (env[0].longj == 1)
    {
        if (env[0].ret == 0)
        {
            env[0].ret = 1;
        }
        return (env[0].ret);
    }
    else
    {
        return (0);
    }
}

void longjmp(jmp_buf env, int val)
{
    env[0].longj = 1;
    env[0].ret = val;
   
    return;
}
