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

#include "string.h"

#ifdef memmove
#undef memmove
#endif

void *memmove(void *s1, const void *s2, tamanho_t n)
{
    char *p = s1;
    const char *cs2 = s2;
    tamanho_t x;

    if (p <= cs2)
    {
        for (x=0; x < n; x++)
        {
            *p = *cs2;
            p++;
            cs2++;
        }
    }
    else
    {
        if (n != 0)
        {
            for (x=n-1; x > 0; x--)
            {
                *(p+x) = *(cs2+x);
            }
            *(p+x) = *(cs2+x);
        }
    }
    return (s1);
}

#ifdef strcpy
#undef strcpy
#endif

char *strcpy(char *s1, const char *s2)
{
    char *p = s1;

    while ((*p++ = *s2++) != '\0') ;
    return (s1);
}

#ifdef strncpy
#undef strncpy
#endif

char *strncpy(char *s1, const char *s2, tamanho_t n)
{
    char *p = s1;
    tamanho_t x;

    for (x=0; x < n; x++)
    {
        *p = *s2;
        if (*s2 == '\0') break;
        p++;
        s2++;
    }
    for (; x < n; x++)
    {
        *p++ = '\0';
    }
    return (s1);
}

#ifdef strcat
#undef strcat
#endif
char *strcat(char *s1, const char *s2)
{
    char *p = s1;

    while (*p != '\0') p++;
    while ((*p = *s2) != '\0')
    {
        p++;
        s2++;
    }
    return (s1);
}

#ifdef strncat
#undef strncat
#endif
char *strncat(char *s1, const char *s2, tamanho_t n)
{
    char *p = s1;
    tamanho_t x = 0;

    while (*p != '\0') p++;
    while ((*s2 != '\0') && (x < n))
    {
        *p = *s2;
        p++;
        s2++;
        x++;
    }
    *p = '\0';
    return (s1);
}

#ifdef memcmp
#undef memcmp
#endif
int memcmp(const void *s1, const void *s2, tamanho_t n)
{
    const unsigned char *p1;
    const unsigned char *p2;
    tamanho_t x = 0;

    p1 = (const unsigned char *)s1;
    p2 = (const unsigned char *)s2;
    while (x < n)
    {
        if (p1[x] < p2[x]) return (-1);
        else if (p1[x] > p2[x]) return (1);
        x++;
    }
    return (0);
}

#ifdef strcmp
#undef strcmp
#endif
int strcmp(const char *s1, const char *s2)
{
    const unsigned char *p1;
    const unsigned char *p2;

    p1 = (const unsigned char *)s1;
    p2 = (const unsigned char *)s2;
    while (*p1 != '\0')
    {
        if (*p1 < *p2) return (-1);
        else if (*p1 > *p2) return (1);
        p1++;
        p2++;
    }
    if (*p2 == '\0') return (0);
    else return (-1);
}

#ifdef strcoll
#undef strcoll
#endif
int strcoll(const char *s1, const char *s2)
{
    return (strcmp(s1, s2));
}

#ifdef strncmp
#undef strncmp
#endif
int strncmp(const char *s1, const char *s2, tamanho_t n)
{
    const unsigned char *p1;
    const unsigned char *p2;
    tamanho_t x = 0;

    p1 = (const unsigned char *)s1;
    p2 = (const unsigned char *)s2;
    while (x < n)
    {
        if (p1[x] < p2[x]) return (-1);
        else if (p1[x] > p2[x]) return (1);
        else if (p1[x] == '\0') return (0);
        x++;
    }
    return (0);
}

#ifdef strxfrm
#undef strxfrm
#endif
tamanho_t strxfrm(char *s1, const char *s2, tamanho_t n)
{
    tamanho_t oldlen;

    oldlen = tamanhostring(s2);
    if (oldlen < n)
    {
        memcpy(s1, s2, oldlen);
        s1[oldlen] = '\0';
    }
    return (oldlen);
}

#ifdef memchr
#undef memchr
#endif
void *memchr(const void *s, int c, tamanho_t n)
{
    const unsigned char *p;
    tamanho_t x = 0;

    p = (const unsigned char *)s;
    while (x < n)
    {
        if (*p == (unsigned char)c) return ((void *)p);
        p++;
        x++;
    }
    return (NULL);
}

#ifdef strchr
#undef strchr
#endif
char *strchr(const char *s, int c)
{
    while (*s != '\0')
    {
        if (*s == (char)c) return ((char *)s);
        s++;
    }
    if (c == '\0') return ((char *)s);
    return (NULL);
}

#ifdef strcspn
#undef strcspn
#endif
tamanho_t strcspn(const char *s1, const char *s2)
{
    const char *p1;
    const char *p2;

    p1 = s1;
    while (*p1 != '\0')
    {
        p2 = s2;
        while (*p2 != '\0')
        {
            if (*p1 == *p2) return ((tamanho_t)(p1 - s1));
            p2++;
        }
        p1++;
    }
    return ((tamanho_t)(p1 - s1));
}

#ifdef strpbrk
#undef strpbrk
#endif
char *strpbrk(const char *s1, const char *s2)
{
    const char *p1;
    const char *p2;

    p1 = s1;
    while (*p1 != '\0')
    {
        p2 = s2;
        while (*p2 != '\0')
        {
            if (*p1 == *p2) return ((char *)p1);
            p2++;
        }
        p1++;
    }
    return (NULL);
}

#ifdef strrchr
#undef strrchr
#endif
char *strrchr(const char *s, int c)
{
    const char *p;

    p = s + tamanhostring(s);
    while (p >= s)
    {
        if (*p == (char)c) return ((char *)p);
        p--;
    }
    return (NULL);
}

#ifdef strspn
#undef strspn
#endif
tamanho_t strspn(const char *s1, const char *s2)
{
    const char *p1;
    const char *p2;

    p1 = s1;
    while (*p1 != '\0')
    {
        p2 = s2;
        while (*p2 != '\0')
        {
            if (*p1 == *p2) break;
            p2++;
        }
        if (*p2 == '\0') return ((tamanho_t)(p1 - s1));
        p1++;
    }
    return ((tamanho_t)(p1 - s1));
}


#ifdef strstr
#undef strstr
#endif
char *strstr(const char *s1, const char *s2)
{
    const char *p = s1, *p1, *p2 = s2;

    while (*p)
    {
        if (*p == *s2)
        {
            p1 = p;
            p2 = s2;
            while ((*p2 != '\0') && (*p1++ == *p2++)) ;
            if (*p2 == '\0')
            {
                return (char *)p;
            }
        }
        p++;
    }
    return NULL;
}

#ifdef strtok
#undef strtok
#endif
char *strtok(char *s1, const char *s2)
{
    static char *antigo = NULL;
    char *p;
    tamanho_t tamanho;
    tamanho_t restante;

    if (s1 != NULL) antigo = s1;
    if (antigo == NULL) return (NULL);
    p = antigo;
    tamanho = strspn(p, s2);
    restante = tamanhostring(p);
    if (restante <= tamanho) { antigo = NULL; return (NULL); }
    p += tamanho;
    tamanho = strcspn(p, s2);
    restante = tamanhostring(p);
    if (restante <= tamanho) { antigo = NULL; return (p); }
    *(p + tamanho) = '\0';
    antigo = p + tamanho + 1;
    return (p);
}

#ifdef memset
#undef memset
#endif
void *memset(void *s, int c, tamanho_t n)
{
    tamanho_t x = 0;

    for (x = 0; x < n; x++)
    {
        *((char *)s + x) = (unsigned char)c;
    }
    return (s);
}

#ifdef strerror
#undef strerror
#endif
char *strerror(int errnum)
{
    if (errnum == 0) return ("Nenhum erro ocorreu.\n");
    else return ("Um erro ocorreu.\n");
}

#ifdef tamanhostring
#undef tamanhostring
#endif
tamanho_t tamanhostring(const char *s)
{
    const char *p;

    p = s;
    while (*p != '\0') p++;
    return ((tamanho_t)(p - s));
}

#ifndef USE_ASSEMBLER
#ifdef memcpy
#undef memcpy
#endif
#ifndef __32BIT__
void *memcpy(void *s1, const void *s2, tamanho_t n)
{
    register const unsigned char *f = s2;
    register const unsigned char *fe;
    register unsigned char *t = s1;

    fe = f + n;
    while (f != fe)
    {
        *t++ = *f++;
    }
    return (s1);
}
#else
void *memcpy(void *s1, const void *s2, tamanho_t n)
{
    register unsigned int *p = (unsigned int *)s1;
    register unsigned int *cs2 = (unsigned int *)s2;
    register unsigned int *endi;

    endi = (unsigned int *)((char *)p + (n & ~0x03));
    while (p != endi)
    {
        *p++ = *cs2++;
    }
    switch (n & 0x03)
    {
        case 0:
            break;
        case 1:
            *(char *)p = *(char *)cs2;
            break;
        case 2:
            *(char *)p = *(char *)cs2;
            p = (unsigned int *)((char *)p + 1);
            cs2 = (unsigned int *)((char *)cs2 + 1);
            *(char *)p = *(char *)cs2;
            break;
        case 3:
            *(char *)p = *(char *)cs2;
            p = (unsigned int *)((char *)p + 1);
            cs2 = (unsigned int *)((char *)cs2 + 1);
            *(char *)p = *(char *)cs2;
            p = (unsigned int *)((char *)p + 1);
            cs2 = (unsigned int *)((char *)cs2 + 1);
            *(char *)p = *(char *)cs2;
            break;
    }
    return (s1);
}
#endif /* 32BIT */
#endif /* USE_ASSEMBLER */
