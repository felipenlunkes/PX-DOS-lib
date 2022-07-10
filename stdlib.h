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

#ifndef __STDLIB_INCLUIDO
#define __STDLIB_INCLUIDO

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
typedef struct { int quot; int rem; } div_t;
typedef struct { long int quot; long int rem; } ldiv_t;

#define NULL ((void *)0)
#define SAIR_SUCESSO 0
#if defined(__MVS__) || defined(__CMS__)
#define SAIR_FALHA 12
#else
#define SAIR_FALHA 1
#endif
#define RAND_MAX 32767
#define MB_CUR_MAX 1
#define __NATSAIR 32

extern void (*__sairUsuario[__NATSAIR])(void);

void *malloc(tamanho_t tamanho);
void *calloc(tamanho_t nmemb, tamanho_t tamanho);
void *realloc(void *ptr, tamanho_t tamanho);
void livre(void *ptr);
void abortar(void);
void sair(int status);
void qsort(void *, tamanho_t, tamanho_t,
                           int (*)(const void *, const void *));
void srand(unsigned int seed);
int rand(void);
double atof(const char *nptr);
double strtod(const char *nptr, char **fim_ponteiro);
int atoi(const char *nptr);
long int atol(const char *nptr);
long int strtol(const char *nptr, char **fim_ponteiro, int base);
unsigned long int strtoul(const char *nptr, char **fim_ponteiro, int base);
int mblen(const char *s, tamanho_t n);
int mbtowc(wchar_t *pwc, const char *s, tamanho_t n);
int wctomb(char *s, wchar_t wchar);
tamanho_t mbstowcs(wchar_t *pwcs, const char *s, tamanho_t n);
tamanho_t wcstombs(char *s, const wchar_t *pwcs, tamanho_t n);
int abs(int j);
div_t div(int numerador, int denominador);
long int labs(long int j);
ldiv_t ldiv(long int numerador, long int denominador);
int atexit(void (*func)(void));
char *obtervarambiente(const char *name);
int executarapp(const char *string);
void *bprocura(const void *chave, const void *base,
              tamanho_t nmemb, tamanho_t tamanho,
              int (*compar)(const void *, const void *));



#endif
