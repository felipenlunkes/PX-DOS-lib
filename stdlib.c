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

#include "stdlib.h"
#include "sinal.h"
#include "string.h"
#include "caracter.h"


#ifdef __PXDOS__
#define __MSDOS__
#endif



#ifdef __MSDOS__

#define CTYP


void CTYP __alocarmem(tamanho_t tamanho, void **ptr);
void CTYP __memorialivre(void *ptr);
extern unsigned char *__ponteirodeambiente;
void CTYP __exec(char *cmd, void *env);
#endif

void (*__sairUsuario[__NATSAIR])(void);

void *malloc(tamanho_t tamanho)
{

#ifdef __MSDOS__
    void *ptr;

    __alocarmem(tamanho, &ptr);
    return (ptr);
	
#endif

}

void *calloc(tamanho_t nmemb, tamanho_t tamanho)
{
    void *ptr;
    tamanho_t total;

    if (nmemb == 1)
    {
        total = tamanho;
    }
    else if (tamanho == 1)
    {
        total = nmemb;
    }
    else
    {
        total = nmemb * tamanho;
    }
    ptr = malloc(total);
    if (ptr != NULL)
    {
        memset(ptr, '\0', total);
    }
    return (ptr);
}

void *realloc(void *ptr, tamanho_t tamanho)
{
    char *novoPonteiro;
    tamanho_t tamanhoAntigo;

    if (tamanho == 0)
    {
        livre(ptr);
        return (NULL);
    }

    novoPonteiro = malloc(tamanho);
    if (novoPonteiro == NULL)
    {
        return (NULL);
    }
    if (ptr != NULL)
    {
        tamanhoAntigo = *((tamanho_t *)ptr - 1);
        if (tamanhoAntigo < tamanho)
        {
            tamanho = tamanhoAntigo;
        }
        memcpy(novoPonteiro, ptr, tamanho);
        livre(ptr);
    }
    return (novoPonteiro);
}

void livre(void *ptr)
{

#ifdef __MSDOS__
    if (ptr != NULL)
    {
        __memorialivre(ptr);
    }
#endif

    return;
}

void abortar(void)
{
    raise(SIGABRT);
    sair(SAIR_FALHA);
#if !defined(__EMX__) && !defined(__GCC__) && !defined(__WIN32__)
    return;
#endif
}

#if !defined(__EMX__) && !defined(__GCC__) && !defined(__WIN32__)
void __sair(int status);
#else

#endif

void sair(int status)
{
    __sair(status);
#if !defined(__EMX__) && !defined(__GCC__) && !defined(__WIN32__)
    return;
#endif
}

static void memswp(char *i, char *j, tamanho_t tamanho)
{
     char tmp;

     while (tamanho-- > 0)
     {
         tmp = *i;
         *i++ = *j;
         *j++ = tmp;
     };
     return;
}

#define T 7

void qsort(void *base,
           tamanho_t nmemb,
           tamanho_t tamanho,
           int (*compar)(const void *, const void *))
{
    char * i;
    char * j;
    tamanho_t thresh     = T * tamanho;
    char * base_      = (char *)base;
    char * limite      = base_ + nmemb * tamanho;

    if ( ( nmemb == 0 ) || ( tamanho == 0 ) || ( base == NULL ) )
    {
        return;
    }

    for ( ;; )
    {
        if ( limite - base_ > thresh ) 
        {
          
            i = base_ + tamanho;
            j = limite - tamanho;
          
            memswp( ( ( ( (tamanho_t)( limite - base_ ) ) / tamanho ) / 2 )
                    * tamanho + base_, base_, tamanho );
            if ( compar( i, j ) > 0 ) memswp( i, j, tamanho );
            if ( compar( base_, j ) > 0 ) memswp( base_, j, tamanho );
            if ( compar( i, base_ ) > 0 ) memswp( i, base_, tamanho );
           
            for ( ;; )
            {
                do
                {
                    
                    i += tamanho;
                } while ( compar( i, base_ ) < 0 );
                do
                {
                   
                    j -= tamanho;
                } while ( compar( j, base_ ) > 0 );
                if ( i > j )
                {
                 
                    break;
                }
              
                memswp( i, j, tamanho );
            }
          
            memswp( base_, j, tamanho );
           
            if ( j - base_ > limite - i )
            {
            
                qsort( base, ( j - base_ ) / tamanho, tamanho, compar );
                base_ = i;
            }
            else
            {
               
                qsort( i, ( limite - i ) / tamanho, tamanho, compar );
                limite = j;
            }
        }
        else 
        {
            for ( j = base_, i = j + tamanho; i < limite; j = i, i += tamanho )
            {
                for ( ; compar( j, j + tamanho ) > 0; j -= tamanho )
                {
                    memswp( j, j + tamanho, tamanho );
                    if ( j == base_ )
                    {
                        break;
                    }
                }
            }
            break;
        }
    }
    return;
}


static unsigned long minhaSemente = 1;

void srand(unsigned int seed)
{
    minhaSemente = seed;
    return;
}

int rand(void)
{
    int ret;

#if defined(__MVS__) || defined(__CMS__)
    
    minhaSemente = minhaSemente * 1103515245UL;
    ret = (int)(((minhaSemente + 12345) >> 16) & 0x8fff);
#else
    minhaSemente = minhaSemente * 1103515245UL + 12345;
    ret = (int)((minhaSemente >> 16) & 0x8fff);
#endif
    return (ret);
}

double atof(const char *nptr)
{
    return (strtod(nptr, (char **)NULL));
}

double strtod(const char *nptr, char **fim_ponteiro)
{
    double x = 0.0;
    double xs= 1.0;
    double es = 1.0;
    double xf = 0.0;
    double xd = 1.0;

    while( eespaco( (unsigned char)*nptr ) ) ++nptr;
    if(*nptr == '-')
    {
        xs = -1;
        nptr++;
    }
    else if(*nptr == '+')
    {
        nptr++;
    }


    while (1)
    {
        if (eumdigito((unsigned char)*nptr))
        {
            x = x * 10 + (*nptr - '0');
            nptr++;
        }
        else
        {
            x = x * xs;
            break;
        }
    }
    if (*nptr == '.')
    {
        nptr++;
        while (1)
        {
            if (eumdigito((unsigned char)*nptr))
            {
                xf = xf * 10 + (*nptr - '0');
                xd = xd * 10;
            }
            else
            {
                x = x + xs * (xf / xd);
                break;
            }
            nptr++;
        }
    }
    if ((*nptr == 'e') || (*nptr == 'E'))
    {
        nptr++;
        if (*nptr == '-')
        {
            es = -1;
            nptr++;
        }
        xd = 1;
        xf = 0;
        while (1)
        {
            if (eumdigito((unsigned char)*nptr))
            {
                xf = xf * 10 + (*nptr - '0');
                nptr++;
            }
            else
            {
                while (xf > 0)
                {
                    xd *= 10;
                    xf--;
                }
                if (es < 0.0)
                {
                    x = x / xd;
                }
                else
                {
                    x = x * xd;
                }
                break;
            }
        }
    }
    if (fim_ponteiro != NULL)
    {
        *fim_ponteiro = (char *)nptr;
    }
    return (x);
}

int atoi(const char *nptr)
{
    return ((int)strtol(nptr, (char **)NULL, 10));
}

long int atol(const char *nptr)
{
    return (strtol(nptr, (char **)NULL, 10));
}

long int strtol(const char *nptr, char **fim_ponteiro, int base)
{
    long x = 0;
    int naoDecidido = 0;
    int neg = 0;

    if (base == 0)
    {
        naoDecidido = 1;
    }
    if (*nptr == '-')
    {
        neg = 1;
        nptr++;
    }
    while (1)
    {
        if (eumdigito((unsigned char)*nptr))
        {
            if (base == 0)
            {
                if (*nptr == '0')
                {
                    base = 8;
                }
                else
                {
                    base = 10;
                    naoDecidido = 0;
                }
            }
            x = x * base + (*nptr - '0');
            nptr++;
        }
        else if (ealpha((unsigned char)*nptr))
        {
            if ((*nptr == 'X') || (*nptr == 'x'))
            {
                if ((base == 0) || ((base == 8) && naoDecidido))
                {
                    base = 16;
                    naoDecidido = 0;
                    nptr++;
                }
                else
                {
                    break;
                }
            }
            else
            {
                x = x * base + (paramaiusculo((unsigned char)*nptr) - 'A') + 10;
                nptr++;
            }
        }
        else
        {
            break;
        }
    }
    if (neg)
    {
        x = -x;
    }
    if (fim_ponteiro != NULL)
    {
        *fim_ponteiro = (char *)nptr;
    }
    return (x);
}

unsigned long int strtoul(const char *nptr, char **fim_ponteiro, int base)
{
    unsigned long x = 0;

    while (1)
    {
        if (eumdigito((unsigned char)*nptr))
        {
            x = x * base + (*nptr - '0');
            nptr++;
        }
        else if (ealpha((unsigned char)*nptr) && (base > 10))
        {
            x = x * base + (paramaiusculo((unsigned char)*nptr) - 'A') + 10;
            nptr++;
        }
        else
        {
            break;
        }
    }
    if (fim_ponteiro != NULL)
    {
        *fim_ponteiro = (char *)nptr;
    }
    return (x);
}

int mblen(const char *s, tamanho_t n)
{
    if (s == NULL)
    {
        return (0);
    }
    if (n == 1)
    {
        return (1);
    }
    else
    {
        return (-1);
    }
}

int mbtowc(wchar_t *pwc, const char *s, tamanho_t n)
{
    if (s == NULL)
    {
        return (0);
    }
    if (n == 1)
    {
        if (pwc != NULL)
        {
            *pwc = *s;
        }
        return (1);
    }
    else
    {
        return (-1);
    }
}

int wctomb(char *s, wchar_t wchar)
{
    if (s != NULL)
    {
        *s = wchar;
        return (1);
    }
    else
    {
        return (0);
    }
}

tamanho_t mbstowcs(wchar_t *pwcs, const char *s, tamanho_t n)
{
    strncpy((char *)pwcs, s, n);
    if (tamanhostring(s) >= n)
    {
        return (n);
    }
    return (tamanhostring((char *)pwcs));
}

tamanho_t wcstombs(char *s, const wchar_t *pwcs, tamanho_t n)
{
    strncpy(s, (const char *)pwcs, n);
    if (tamanhostring((const char *)pwcs) >= n)
    {
        return (n);
    }
    return (tamanhostring(s));
}

#ifdef abs
#undef abs
#endif
int abs(int j)
{
    if (j < 0)
    {
        j = -j;
    }
    return (j);
}

div_t div(int numerador, int denominador)
{
    div_t x;

    x.quot = numerador / denominador;
    x.rem = numerador % denominador;
    return (x);
}

#ifdef labs
#undef labs
#endif
long int labs(long int j)
{
    if (j < 0)
    {
        j = -j;
    }
    return (j);
}

ldiv_t ldiv(long int numerador, long int denominador)
{
    ldiv_t x;

    x.quot = numerador / denominador;
    x.rem = numerador % denominador;
    return (x);
}

int atsair(void (*func)(void))
{
    int x;

    for (x = 0; x < __NATSAIR; x++)
    {
        if (__sairUsuario[x] == 0)
        {
            __sairUsuario[x] = func;
            return (0);
        }
    }
    return (-1);
}

char *obtervarambiente(const char *name)
{

#if defined(__MSDOS__) || defined(__WIN32__)
    char *env;
    tamanho_t lenn;


    env = (char *)__ponteirodeambiente;

    lenn = tamanhostring(name);
    while (*env != '\0')
    {
        if (strncmp(env, name, lenn) == 0)
        {
            if (env[lenn] == '=')
            {
                return (&env[lenn + 1]);
            }
        }
        env = env + tamanhostring(env) + 1;
    }
#endif
    return (NULL);
}


int executarapp(const char *string)
{


#ifdef __MSDOS__
    static unsigned char cmdt[140];
    static struct {
        int env;
        unsigned char *linhaDeComando;
        char *fcb1;
        char *fcb2;
    } blocoParametros = { 0, cmdt, NULL, NULL };
    tamanho_t tamanho;
    char *cmd;

    if (string == NULL)
    {
        return (1);
    }
    tamanho = tamanhostring(string);
    cmdt[0] = (unsigned char)(tamanho + 3);
    memcpy(&cmdt[1], "/c ", 3);
    memcpy(&cmdt[4], string, tamanho);
    memcpy(&cmdt[tamanho + 4], "\r", 2);
    cmd = obtervarambiente("COMSPEC");
    if (cmd == NULL)
    {
        cmd = "\\comando.epx";
    }
    __exec(cmd, &blocoParametros);
    return (0);
#endif

}

void *bprocura(const void *chave, const void *base,
              tamanho_t nmemb, tamanho_t tamanho,
              int (*compar)(const void *, const void *))
{
    tamanho_t tentativa;
    int res;
    const void *ptr;

    while (nmemb > 0)
    {
        tentativa = nmemb / 2;
        ptr = (void *)((char *)base + tentativa * tamanho);
        res = compar(ptr, chave);
        if (res == 0)
        {
            return ((void *)ptr);
        }
        else if (res < 0)
        {
            nmemb = nmemb - tentativa - 1;
            base = (const void *)((const char *)ptr + tamanho);
        }
        else
        {
            nmemb = tentativa;
        }
    }
    return (NULL);
}
