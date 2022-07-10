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

#include "sinal.h"
#include "stdlib.h"

static void (*manipuladores[])(int) = {
    __sigdfl,
    __sigdfl,
    __sigdfl,
    __sigdfl,
    __sigdfl,
    __sigdfl,
    __sigdfl };

void __sigdfl(int sig);
void __sigerr(int sig);
void __sigign(int sig);

#define SIG_DFL __sigdfl
#define SIG_ERR __sigerr
#define SIG_IGN __sigign

void (*signal(int sig, void (*func)(int)))(int)
{
    manipuladores[sig] = func;
    return (func);
}


int raise(int sig)
{
    (manipuladores[sig])(sig);
    return (0);
}

void __sigdfl(int sig)
{
    manipuladores[sig] = SIG_DFL;
    if (sig == SIGABRT)
    {
        sair(SAIR_FALHA);
    }
    return;
}

void __sigerr(int sig)
{
    (void)sig;
    return;
}

void __sigign(int sig)
{
    (void)sig;
    return;
}

