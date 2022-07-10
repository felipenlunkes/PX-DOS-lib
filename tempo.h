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

#ifndef __TIME_INCLUIDO
#define __TIME_INCLUIDO

#define CLOCKS_POR_SEGUNDO 1000
#define NULL ((void *)0)

typedef unsigned int relogio_t;

#ifndef __TAMANHO_T_DEFINIDO
#define __TAMANHO_T_DEFINIDO

typedef unsigned int tamanho_t;

#endif

typedef unsigned long tempo_t;

struct tm
{
    int tm_sec;
    int tm_minutos;
    int tm_hora;
    int tm_mdia;
    int tm_mes;
    int tm_ano;
    int tm_wdia;
    int tm_ydia;
    int tm_eHorarioDeVerao;
};

tempo_t hora(tempo_t *timer);

relogio_t relogio(void);

double diferencaTempo(tempo_t tempo1, tempo_t tempo0);

tempo_t fazertempo(struct tm *ponteirodeTempo);

char *asctempo(const struct tm *ponteirodeTempo);

char *horac(const tempo_t *timer);

struct tm *horagmt(const tempo_t *timer);

struct tm *horalocal(const tempo_t *timer);

tamanho_t strfTempo(char *s, tamanho_t tamanhoMaximo,
                const char *formato_STDIO, const struct tm *ponteirodeTempo);

#endif
