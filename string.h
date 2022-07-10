//******************************************************************
//
// Biblioteca de Fun��es C ara PX-DOS (LIBPXDOSC)
//
// Vers�o 1.6.rc2
//
// Compat�vel com PX-DOS 0.1.1 e 0.9.0+
//
// Copyright (C) 2013-2016 Felipe Miguel Nery Lunkes
//
//******************************************************************

#ifndef __STRING_INCLUIDO
#define __STRING_INCLUIDO

#ifndef __TAMANHO_T_DEFINIDO
#define __TAMANHO_T_DEFINIDO

typedef unsigned int tamanho_t;

#endif

#define NULL ((void *)0)

void *memcpy(void *s1, const void *s2, tamanho_t n);
void *memmove(void *s1, const void *s2, tamanho_t n);
char *strcpy(char *s1, const char *s2);
char *strncpy(char *s1, const char *s2, tamanho_t n);
char *strcat(char *s1, const char *s2);
char *strncat(char *s1, const char *s2, tamanho_t n);
int memcmp(const void *s1, const void *s2, tamanho_t n);
int strcmp(const char *s1, const char *s2);
int strcoll(const char *s1, const char *s2);
int strncmp(const char *s1, const char *s2, tamanho_t n);
tamanho_t strxfrm(char *s1, const char *s2, tamanho_t n);
void *memchr(const void *s, int c, tamanho_t n);
char *strchr(const char *s, int c);
tamanho_t strcspn(const char *s1, const char *s2);
char *strpbrk(const char *s1, const char *s2);
char *strrchr(const char *s, int c);
tamanho_t strspn(const char *s1, const char *s2);
char *strstr(const char *s1, const char *s2);
char *strtok(char *s1, const char *s2);
void *memset(void *s, int c, tamanho_t n);
char *strerror(int errnum);
tamanho_t tamanhostring(const char *s);

#ifdef __WATCOMC__
#ifdef __INLINE_FUNCTIONS__
#pragma intrinsic(memchr, memcmp, memcpy, strcat, strcpy, tamanhostring, strchr)
#endif
#endif

#ifdef __IBMC__
char * _Builtin __strcat(char *s1, const char *s2);
#define strcat(s1,s2) (__strcat((s1),(s2)))
char * _Builtin __strchr(const char *s, int c);
#define strchr(s,c) (__strchr((s),(c)))
int _Builtin __strcmp(const char *s1, const char *s2);
#define strcmp(s1,s2) (__strcmp((s1),(s2)))
int _Builtin __strcpy(char *s1, const char *s2);
#define strcpy(s1,s2) (__strcpy((s1),(s2)))
tamanho_t _Builtin __tamanhostring(const char *s);
#define tamanhostring(s) (__tamanhostring((s)))
char * _Builtin __strncat(char *s1, const char *s2, tamanho_t n);
#define strncat(s1,s2,n) (__strncat((s1),(s2),(n)))
int _Builtin __strncmp(const char *s1, const char *s2, tamanho_t n);
#define strncmp(s1,s2,n) (__strncmp((s1),(s2),(n)))
char * _Builtin __strncpy(char *s1, const char *s2, tamanho_t n);
#define strncpy(s1,s2,n) (__strncpy((s1),(s2),(n)))
char * _Builtin __strrchr(const char *s, int c);
#define strrchr(s,c) (__strrchr((s),(c)))
void * _Builtin __memcpy(void *s1, const void *s2, tamanho_t n);
#define memcpy(a,b,c) (__memcpy((a),(b),(c)))
void * _Builtin __memchr(const void *s, int c, tamanho_t n);
#define memchr(s,c,n) (__memchr((s),(c),(n)))
int _Builtin __memcmp(const void *s1, const void *s2, tamanho_t n);
#define memcmp(s1,s2,n) (__memcmp((s1),(s2),(n)))
void * _Builtin __memset(void *s, int c, tamanho_t n);
#define memset(s,c,n) (__memset((s),(c),(n)))
void * _Builtin __memmove(void *s1, const void *s2, tamanho_t n);
#define memmove(s1,s2,n) (__memmove((s1),(s2),(n)))
#endif


#if 0
#define strcat(s1,s2) (__builtin_strcat((s1),(s2)))
#define strchr(s,c) (__builtin_strchr((s),(c)))
#define strcmp(s1,s2) (__builtin_strcmp((s1),(s2)))
#define strcpy(s1,s2) (__builtin_strcpy((s1),(s2)))
#define tamanhostring(s) (__builtin_tamanhostring((s)))
#define strncat(s1,s2,n) (__builtin_strncat((s1),(s2),(n)))
#define strncmp(s1,s2,n) (__builtin_strncmp((s1),(s2),(n)))
#define strncpy(s1,s2,n) (__builtin_strncpy((s1),(s2),(n)))
#define strrchr(s,c) (__builtin_strrchr((s),(c)))
#define memset(s,c,n) (__builtin_memset((s),(c),(n)))
#define strstr(s1,s2) (__builtin_strstr((s1),(s2)))
#define strpbrk(s1,s2) (__builtin_strpbrk((s1),(s2)))
#define strspn(s1,s2) (__builtin_strspn((s1),(s2)))
#define strcspn(s1,s2) (__builtin_strcspn((s1),(s2)))
#endif

#ifdef __BORLANDC__
#ifdef __INLINE_FUNCTIONS__
void *__memcpy__(void *s1, const void *s2, tamanho_t n);
#define memcpy(a,b,c) (__memcpy__((a),(b),(c)))
void *__memchr__(const void *s, int c, tamanho_t n);
#define memchr(s,c,n) (__memchr__((s),(c),(n)))
int __memcmp__(const void *s1, const void *s2, tamanho_t n);
#define memcmp(s1,s2,n) (__memcmp__((s1),(s2),(n)))
void *__memset__(void *s, int c, tamanho_t n);
#define memset(s,c,n) (__memset__((s),(c),(n)))
char *__strcat__(char *s1, const char *s2);
#define strcat(s1,s2) (__strcat__((s1),(s2)))
char *__strchr__(const char *s, int c);
#define strchr(s,c) (__strchr__((s),(c)))
int __strcmp__(const char *s1, const char *s2);
#define strcmp(s1,s2) (__strcmp__((s1),(s2)))
char *__strcpy__(char *s1, const char *s2);
#define strcpy(s1,s2) (__strcpy__((s1),(s2)))
tamanho_t __tamanhostring__(const char *s);
#define tamanhostring(s) (__tamanhostring__((s)))
char *__strncat__(char *s1, const char *s2, tamanho_t n);
#define strncat(s1,s2,n) (__strncat__((s1),(s2),(n)))
int __strncmp__(const char *s1, const char *s2, tamanho_t n);
#define strncmp(s1,s2,n) (__strncmp__((s1),(s2),(n)))
char *__strncpy__(char *s1, const char *s2, tamanho_t n);
#define strncpy(s1,s2,n) (__strncpy__((s1),(s2),(n)))
char *__strrchr__(const char *s, int c);
#define strrchr(s,c) (__strrchr__((s),(c)))
#endif
#endif

#endif
