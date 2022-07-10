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

#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "stdarg.h"
#include "caracter.h"
#include "errno.h"
#include "float.h"
#include "limites.h"


#ifdef __PXDOS__
#define __MSDOS__
#endif

#ifdef __MSDOS__

#define CTYP



extern int CTYP __abrir(const char *nome_de_arquivo, int modo, int *errind);
extern int CTYP __ler(int manipular, void *buf, tamanho_t tamanho, int *errind);
extern int CTYP __escrever(int manipular, const void *buf, tamanho_t tamanho, int *errind);
extern void CTYP __obter(int manipular, long offset, int deOnde);
extern void CTYP __fechar(int manipular);
extern void CTYP __remover(const char *nome_de_arquivo);
extern void CTYP __renomear(const char *antigo, const char *new);

#endif



static ARQUIVO ArquivoPermanente_STDIO[3];

#define unused(x) ((void)(x))
#define outch(ch) ((fq == NULL) ? *s++ = (char)ch : putc(ch, fq))
#define inch() ((fp == NULL) ? \
    (ch = (unsigned char)*s++) : (ch = getc(fp)))

ARQUIVO *stdin = &ArquivoPermanente_STDIO[0];
ARQUIVO *stdout = &ArquivoPermanente_STDIO[1];
ARQUIVO *stderr = &ArquivoPermanente_STDIO[2];

ARQUIVO *__Arquivos_Usuario[__N_ARQUIVO];
static ARQUIVO  *meuArquivo;
static int    LocalSobressalente;
static int    err;

static const char *nome_arquivo_lib;
static const char *modus;
static int Tipomodo;
static void ver(void);

static void dblcvt(double num, char cnvtype, tamanho_t nlargura,
                   tamanho_t nprecisao, char *resultado);
static int vvprintf(const char *formato_STDIO, va_list arg, ARQUIVO *fq, char *s);
static int vvscanf(const char *formato_STDIO, va_list arg, ARQUIVO *fp, const char *s);
static void abrir2(void);
static void abrir3(void);
static void EncontrarEspacoSobressalente(void);
static void ChecarModo(void);
static void SOAbrirArquivo(void);

#if !defined(__MVS__) && !defined(__CMS__)

static void escreverDevagarArquivo(const void *ptr,
                       tamanho_t tamanho,
                       tamanho_t nmemb,
                       ARQUIVO *stream,
                       tamanho_t aEscrever,
                       tamanho_t *elemEscrito);
static void escreverDevagarT(const void *ptr,
                        ARQUIVO *stream,
                        tamanho_t aEscrever,
                        tamanho_t *AtualmenteEscrito);
static void escreverDevagarB(const void *ptr,
                        ARQUIVO *stream,
                        tamanho_t aEscrever,
                        tamanho_t *AtualmenteEscrito);
static void lerDevagarT(void *ptr,
                       ARQUIVO *stream,
                       tamanho_t aLer,
                       tamanho_t *LeituraAtual);
static void lerDevagarB(void *ptr,
                       ARQUIVO *stream,
                       tamanho_t aLer,
                       tamanho_t *LeituraAtual);
#endif

static int examine(const char **formt, ARQUIVO *fq, char *s, va_list *arg,
                   int contador_chars);



int printf(const char *formato_STDIO, ...)
{
    va_list arg;
    int ret;

    va_start(arg, formato_STDIO);
    ret = vfprintf(stdout, formato_STDIO, arg);
    va_end(arg);
    return (ret);
}

int fprintf(ARQUIVO *stream, const char *formato_STDIO, ...)
{
    va_list arg;
    int ret;

    va_start(arg, formato_STDIO);
    ret = vfprintf(stream, formato_STDIO, arg);
    va_end(arg);
    return (ret);
}

static void ver(void){
printf("Biblioteca C para PX-DOS (LIBPXDOSC) - Copyright (C) 2013-2016 Felipe Miguel Nery Lunkes");
}

int vfprintf(ARQUIVO *stream, const char *formato_STDIO, va_list arg)
{
    int ret;

    stream->TextoRapido = 0;
    ret = vvprintf(formato_STDIO, arg, stream, NULL);
    return (ret);
}

ARQUIVO *abrir(const char *nome_de_arquivo, const char *modo)
{
    nome_arquivo_lib = nome_de_arquivo;
    modus = modo;
    err = 0;
    EncontrarEspacoSobressalente();
    if (!err)
    {
        meuArquivo = malloc(sizeof(ARQUIVO));
        if (meuArquivo == NULL)
        {
            err = 1;
        }
        else
        {
            abrir2();
            if (err)
            {
                livre(meuArquivo);
            }
        }
    }
    if (err)
    {
        meuArquivo = NULL;
    }
    return (meuArquivo);
}

static void abrir2(void)
{
    ChecarModo();
    if (!err)
    {
        strcpy(meuArquivo->StrMODO, modus);
        SOAbrirArquivo();
        if (!err)
        {
            __Arquivos_Usuario[LocalSobressalente] = meuArquivo;
            meuArquivo->Numero_Arquivo_Interno = LocalSobressalente;
            abrir3();
        }
    }
    return;
}

static void abrir3(void)
{
    meuArquivo->intBuffer = malloc(TamanhoBuffer_DEFINE + 8);
    if (meuArquivo->intBuffer == NULL)
    {
        err = 1;
    }
    else
    {
        meuArquivo->EsteBuffer = 0;
        meuArquivo->fbuf = meuArquivo->intBuffer + 2;
        *meuArquivo->fbuf++ = '\0';
        *meuArquivo->fbuf++ = '\0';
        meuArquivo->tamanhoabuf = TamanhoBuffer_DEFINE;
#if !defined(__MVS__) && !defined(__CMS__)
        meuArquivo->TextoRapido = 0;
#endif
        meuArquivo->noNl = 0;
        meuArquivo->fimbuffer = meuArquivo->fbuf + meuArquivo->tamanhoabuf;
        *meuArquivo->fimbuffer = '\n';
#if defined(__MVS__) || defined(__CMS__)
        meuArquivo->ProximoLista = meuArquivo->fbuf;
        meuArquivo->tamanhoabuf = meuArquivo->lrecl;
        meuArquivo->fimbuffer = meuArquivo->fbuf; 
#else
        meuArquivo->ProximoLista = meuArquivo->fimbuffer;
#endif
#if defined(__MVS__) || defined(__CMS__)
        meuArquivo->InicioBufferR = 0;
#else
        meuArquivo->InicioBufferR = -(long)meuArquivo->tamanhoabuf;
#endif
        meuArquivo->ErronoArquivo = 0;
        meuArquivo->eofInd = 0;
        meuArquivo->ungetCh = -1;
        meuArquivo->atualizar = 0;
        meuArquivo->arquivoPermanente = 0;
        meuArquivo->Arquivo_Aberto = 1;
#if !defined(__MVS__) && !defined(__CMS__)
        if (!meuArquivo->modoTexto)
        {
            meuArquivo->BinRapido = 1;
        }
        else
        {
            meuArquivo->BinRapido = 0;
        }
#endif
        meuArquivo->modo = __MODO_LEITURA;
        switch (Tipomodo)
        {
            case 2:
            case 3:
            case 5:
            case 6:
            case 8:
            case 9:
            case 11:
            case 12:
                meuArquivo->InicioBufferR = 0;
                meuArquivo->ProximoLista = meuArquivo->fbuf;
                meuArquivo->modo = __MODO_ESCRITA;
#if defined(__MVS__) || defined(__CMS__)
                meuArquivo->fimbuffer = meuArquivo->fbuf + meuArquivo->tamanhoabuf;
#endif
                break;
        }
        switch (Tipomodo)
        {
            case 7:
            case 8:
            case 10:
            case 11:
            case 12:
                meuArquivo->atualizar = 1;
                break;
        }
    }
    return;
}

static void EncontrarEspacoSobressalente(void)
{
    int x;

    for (x = 0; x < __N_ARQUIVO; x++)
    {
        if (__Arquivos_Usuario[x] == NULL)
        {
            break;
        }
    }
    if (x == __N_ARQUIVO)
    {
        err = 1;
    }
    else
    {
        LocalSobressalente = x;
    }
    return;
}

/* ChecarModo - Interpretar Modo */
/* r = 1 */
/* w = 2 */
/* a = 3 */
/* rb = 4 */
/* wb = 5 */
/* ab = 6 */
/* r+ = 7 */
/* w+ = 8 */
/* a+ = 9 */
/* r+b or rb+ = 10 */
/* w+b or wb+ = 11 */
/* a+b or ab+ = 12 */

static void ChecarModo(void)
{
    if (strncmp(modus, "r+b", 3) == 0)
    {
        Tipomodo = 10;
    }
    else if (strncmp(modus, "rb+", 3) == 0)
    {
        Tipomodo = 10;
    }
    else if (strncmp(modus, "w+b", 3) == 0)
    {
        Tipomodo = 11;
    }
    else if (strncmp(modus, "wb+", 3) == 0)
    {
        Tipomodo = 11;
    }
    else if (strncmp(modus, "a+b", 3) == 0)
    {
        Tipomodo = 12;
    }
    else if (strncmp(modus, "ab+", 3) == 0)
    {
        Tipomodo = 12;
    }
    else if (strncmp(modus, "r+", 2) == 0)
    {
        Tipomodo = 7;
    }
    else if (strncmp(modus, "w+", 2) == 0)
    {
        Tipomodo = 8;
    }
    else if (strncmp(modus, "a+", 2) == 0)
    {
        Tipomodo = 9;
    }
    else if (strncmp(modus, "rb", 2) == 0)
    {
        Tipomodo = 4;
    }
    else if (strncmp(modus, "wb", 2) == 0)
    {
        Tipomodo = 5;
    }
    else if (strncmp(modus, "ab", 2) == 0)
    {
        Tipomodo = 6;
    }
    else if (strncmp(modus, "r", 1) == 0)
    {
        Tipomodo = 1;
    }
    else if (strncmp(modus, "w", 1) == 0)
    {
        Tipomodo = 2;
    }
    else if (strncmp(modus, "a", 1) == 0)
    {
        Tipomodo = 3;
    }
    else
    {
        err = 1;
        return;
    }
    if ((Tipomodo == 4)
        || (Tipomodo == 5)
        || (Tipomodo == 6)
        || (Tipomodo == 10)
        || (Tipomodo == 11)
        || (Tipomodo == 12))
    {
        meuArquivo->modoTexto = 0;
    }
    else
    {
        meuArquivo->modoTexto = 1;
    }
    return;
}

static void SOAbrirArquivo(void)
{

#ifdef __MSDOS__
    int modo;
    int errind;

    if ((Tipomodo == 1) || (Tipomodo == 4))
    {
        modo = 0;
    }
    else if ((Tipomodo == 2) || (Tipomodo == 5))
    {
        modo = 1;
    }
    else
    {
        modo = 2;
    }
    meuArquivo->ManipuladorArquivo = __abrir(nome_arquivo_lib, modo, &errind);
    if (errind)
    {
        err = 1;
        errno = meuArquivo->ManipuladorArquivo;
    }
#endif
#if defined(__MVS__) || defined(__CMS__)
    int modo;
    char *p;
    int tamanho;
#ifdef __CMS__
    char tmpdd[9];
#endif


    if ((Tipomodo == 1) || (Tipomodo == 4))
    {
        modo = 0;
    }
    else if ((Tipomodo == 2) || (Tipomodo == 5))
    {
        modo = 1;
    }
    else
    {
        modo = 2;
    }

    p = strchr(nome_arquivo_lib, ':');
    if ((p != NULL)
        && ((strncmp(nome_arquivo_lib, "dd", 2) == 0)
            || (strncmp(nome_arquivo_lib, "DD", 2) == 0)))
    {
        p++;
    }
    else

#ifdef __CMS__
    {
        char novoNomeDeArquivo[ARQUIVOS_MAXIMOS];


        strcpy(novoNomeDeArquivo, nome_arquivo_lib);
        p = novoNomeDeArquivo;
        while (*p != '\0')
        {
            *p = paramaiusculo((unsigned char)*p);
            p++;
        }
        sprintf(tmpdd, "PDP%03dHD", LocalSobressalente);
        filedef(tmpdd, novoNomeDeArquivo, modo);
        p = tmpdd;
    }
#else
    {
        p = (char *)nome_arquivo_lib;
    }
#endif
    strcpy(meuArquivo->nome_dd, "        ");
    tamanho = strcspn(p, "(");
    if (tamanho > 8)
    {
        tamanho = 8;
    }
    memcpy(meuArquivo->nome_dd, p, tamanho);
    p = meuArquivo->nome_dd;
    while (*p != '\0')
    {
        *p = paramaiusculo((unsigned char)*p);
        p++;
    }

    p = strchr(nome_arquivo_lib, '(');
    if (p != NULL)
    {
        p++;
        strcpy(meuArquivo->pxdosMem, "        ");
        tamanho = strcspn(p, ")");
        if (tamanho > 8)
        {
            tamanho = 8;
        }
        memcpy(meuArquivo->pxdosMem, p, tamanho);
        p = meuArquivo->pxdosMem;
        while (*p != '\0')
        {
            *p = paramaiusculo((unsigned char)*p);
            p++;
        }
        p = meuArquivo->pxdosMem;
    }
    meuArquivo->ManipuladorArquivo =
        __aopen(meuArquivo->nome_dd, modo, &meuArquivo->recfm, &meuArquivo->lrecl, p);

 
    if ((int)meuArquivo->ManipuladorArquivo <= 0)
    {
        err = 1;
        errno = -(int)meuArquivo->ManipuladorArquivo;
        return;
    }

    if ((Tipomodo == 4) || (Tipomodo == 5))
    {
        meuArquivo->estilo = 0; /* binário */
    }
    else
    {
        meuArquivo->estilo = 2; /* texto */
    }
    meuArquivo->estilo += meuArquivo->recfm;
    if (meuArquivo->estilo == VARIABLE_TEXT)
    {
        meuArquivo->TextoRapido = 1;
    }
    else
    {
        meuArquivo->TextoRapido = 0;
    }
    if (meuArquivo->estilo == FIXED_BINARY)
    {
        meuArquivo->BinRapido = 1;
    }
    else
    {
        meuArquivo->BinRapido = 0;
    }
#endif
    return;
}

int fechar(ARQUIVO *stream)
{


    if (!stream->Arquivo_Aberto)
    {
        return (EOF);
    }
    fflush(stream);

#ifdef __MSDOS__
    __fechar(stream->ManipuladorArquivo);
#endif
    if (!stream->EsteBuffer)
    {
        livre(stream->intBuffer);
    }
    if (!stream->arquivoPermanente)
    {
        __Arquivos_Usuario[stream->Numero_Arquivo_Interno] = NULL;
        livre(stream);
    }
    else
    {
        stream->Arquivo_Aberto = 0;
    }

    return (0);
}

#if !defined(__MVS__) && !defined(__CMS__)
tamanho_t ArquivoLer(void *ptr, tamanho_t tamanho, tamanho_t nmemb, ARQUIVO *stream)
{
    tamanho_t aLer;
    tamanho_t tamanho_leitura;
    tamanho_t LeituraAtual;

#ifdef __MSDOS__
    int errind;
    tamanho_t LeituraTemporaria;
#endif

    if (nmemb == 1)
    {
        aLer = tamanho;
    }
    else if (tamanho == 1)
    {
        aLer = nmemb;
    }
    else
    {
        aLer = tamanho * nmemb;
    }
    if (aLer < stream->tamanhoabuf)
    {
        stream->BinRapido = 0;
    }
    if (stream->ungetCh != -1)
    {
        *--stream->ProximoLista = (char)stream->ungetCh;
        stream->ungetCh = -1;
    }
    if (!stream->BinRapido)
    {
        if (stream->modoTexto)
        {
            lerDevagarT(ptr, stream, aLer, &LeituraAtual);
        }
        else
        {
            if (aLer <= (stream->fimbuffer - stream->ProximoLista))
            {
                memcpy(ptr, stream->ProximoLista, aLer);
                LeituraAtual = aLer;
                stream->ProximoLista += aLer;
            }
            else
            {
                lerDevagarB(ptr, stream, aLer, &LeituraAtual);
            }
        }
        if (nmemb == 1)
        {
            if (LeituraAtual == tamanho)
            {
                tamanho_leitura = 1;
            }
            else
            {
                tamanho_leitura = 0;
            }
        }
        else if (tamanho == 1)
        {
            tamanho_leitura = LeituraAtual;
        }
        else
        {
            tamanho_leitura = LeituraAtual / tamanho;
        }
        return (tamanho_leitura);
    }
    else
    {

#ifdef __MSDOS__
        LeituraTemporaria = __ler(stream->ManipuladorArquivo, ptr, aLer, &errind);
        if (errind)
        {
            errno = LeituraTemporaria;
            LeituraAtual = 0;
            stream->ErronoArquivo = 1;
        }
        else
        {
            LeituraAtual = LeituraTemporaria;
        }
#endif
        if (nmemb == 1)
        {
            if (LeituraAtual == tamanho)
            {
                tamanho_leitura = 1;
            }
            else
            {
                tamanho_leitura = 0;
                stream->eofInd = 1;
            }
        }
        else if (tamanho == 1)
        {
            tamanho_leitura = LeituraAtual;
            if (nmemb != LeituraAtual)
            {
                stream->eofInd = 1;
            }
        }
        else
        {
            tamanho_leitura = LeituraAtual / tamanho;
            if (aLer != LeituraAtual)
            {
                stream->eofInd = 1;
            }
        }
        stream->InicioBufferR += LeituraAtual;
        return (tamanho_leitura);
    }
}



static void lerDevagarT(void *ptr,
                       ARQUIVO *stream,
                       tamanho_t aLer,
                       tamanho_t *LeituraAtual)
{
    int Lendo = 0;
    tamanho_t disponivel;
    tamanho_t necessario;
    char *p;
    tamanho_t obtido;


#ifdef __MSDOS__
    tamanho_t LeituraTemporaria;
    int errind;
#endif

    *LeituraAtual = 0;
    while (!Lendo)
    {
        if (stream->ProximoLista == stream->fimbuffer)
        {
            stream->InicioBufferR += (stream->ProximoLista - stream->fbuf);


#ifdef __MSDOS__
            LeituraTemporaria = __ler(stream->ManipuladorArquivo,
                              stream->fbuf,
                              stream->tamanhoabuf,
                              &errind);
            if (errind)
            {
                errno = LeituraTemporaria;
                LeituraTemporaria = 0;
                stream->ErronoArquivo = 1;
            }
#endif
            if (LeituraTemporaria == 0)
            {
                stream->eofInd = 1;
                break;
            }
            stream->fimbuffer = stream->fbuf + LeituraTemporaria;
            *stream->fimbuffer = '\n';
            stream->ProximoLista = stream->fbuf;
        }
        disponivel = (tamanho_t)(stream->fimbuffer - stream->ProximoLista) + 1;
        necessario = aLer - *LeituraAtual;
        p = memchr(stream->ProximoLista, '\n', disponivel);
        obtido = (tamanho_t)(p - stream->ProximoLista);
        if (necessario < obtido)
        {
            memcpy((char *)ptr + *LeituraAtual, stream->ProximoLista, necessario);
            stream->ProximoLista += necessario;
            *LeituraAtual += necessario;
        }
        else
        {
            memcpy((char *)ptr + *LeituraAtual, stream->ProximoLista, obtido);
            stream->ProximoLista += obtido;
            *LeituraAtual += obtido;
            if (p != stream->fimbuffer)
            {
                if (*(stream->ProximoLista - 1) == '\r')
                {
                    *((char *)ptr + *LeituraAtual - 1) = '\n';
                    stream->ProximoLista++;
                }
                else if (necessario != obtido)
                {
                    *((char *)ptr + *LeituraAtual) = '\n';
                    *LeituraAtual += 1;
                    stream->ProximoLista++;
                }
            }
            else
            {
                if (*(stream->ProximoLista - 1) == '\r')
                {
                    *LeituraAtual -= 1;
                }
            }
        }
        if (*LeituraAtual == aLer)
        {
            Lendo = 1;
        }
    }
    return;
}

static void lerDevagarB(void *ptr,
                       ARQUIVO *stream,
                       tamanho_t aLer,
                       tamanho_t *LeituraAtual)
{
    tamanho_t disponivel;

#ifdef __MSDOS__
    tamanho_t LeituraTemporaria;
    int errind;
#endif

    disponivel = (tamanho_t)(stream->fimbuffer - stream->ProximoLista);
    memcpy(ptr, stream->ProximoLista, disponivel);
    *LeituraAtual = disponivel;
    stream->InicioBufferR += (stream->fimbuffer - stream->fbuf);
    if (aLer >= stream->tamanhoabuf)
    {
        stream->ProximoLista = stream->fimbuffer;
        stream->BinRapido = 1;

#ifdef __MSDOS__
        LeituraTemporaria = __ler(stream->ManipuladorArquivo,
                          (char *)ptr + *LeituraAtual,
                          aLer - *LeituraAtual,
                          &errind);
        if (errind)
        {
            errno = LeituraTemporaria;
            LeituraTemporaria = 0;
            stream->ErronoArquivo = 1;
        }
#endif
        else if (LeituraTemporaria != (aLer - *LeituraAtual))
        {
            stream->eofInd = 1;
        }
        *LeituraAtual += LeituraTemporaria;
        stream->InicioBufferR += LeituraTemporaria;
    }
    else
    {
        tamanho_t restante_STDIO;

        stream->ProximoLista = stream->fbuf;


#ifdef __MSDOS__
        LeituraTemporaria = __ler(stream->ManipuladorArquivo,
                          stream->fbuf,
                          stream->tamanhoabuf,
                          &errind);
        restante_STDIO = aLer - *LeituraAtual;
        if (errind)
        {
            errno = LeituraTemporaria;
            LeituraTemporaria = 0;
            stream->ErronoArquivo = 1;
        }
#endif
        else if (LeituraTemporaria < restante_STDIO)
        {
            stream->eofInd = 1;
        }
        stream->fimbuffer = stream->fbuf + LeituraTemporaria;
        *stream->fimbuffer = '\n';
        disponivel = (tamanho_t)(stream->fimbuffer - stream->ProximoLista);
        if (disponivel > restante_STDIO)
        {
            disponivel = restante_STDIO;
        }
        memcpy((char *)ptr + *LeituraAtual,
               stream->ProximoLista,
               disponivel);
        stream->ProximoLista += disponivel;
        *LeituraAtual += disponivel;
    }
    return;
}
#endif

#if !defined(__MVS__) && !defined(__CMS__)
tamanho_t escArquivo(const void *ptr, tamanho_t tamanho, tamanho_t nmemb, ARQUIVO *stream)
{
    tamanho_t aEscrever;
    tamanho_t elemEscrito;

#ifdef __MSDOS__
    tamanho_t AtualmenteEscrito;
    int errind;
#endif

    if (nmemb == 1)
    {
        aEscrever = tamanho;
    }
    else if (tamanho == 1)
    {
        aEscrever = nmemb;
    }
    else
    {
        aEscrever = tamanho * nmemb;
    }
    if (aEscrever < stream->tamanhoabuf)
    {
        stream->BinRapido = 0;
        if ((stream->TecnicaBuffer == _IONBF) && !stream->modoTexto)
        {
            stream->BinRapido = 1;
        }
    }
    if (!stream->BinRapido)
    {
        escreverDevagarArquivo(ptr, tamanho, nmemb, stream, aEscrever, &elemEscrito);
        return (elemEscrito);
    }
    else
    {

#ifdef __MSDOS__
        AtualmenteEscrito = __escrever(stream->ManipuladorArquivo,
                                ptr,
                                aEscrever,
                                &errind);
        if (errind)
        {
            stream->ErronoArquivo = 1;
            AtualmenteEscrito = 0;
            errno = AtualmenteEscrito;
        }
#endif
        if (nmemb == 1)
        {
            if (AtualmenteEscrito == tamanho)
            {
                elemEscrito = 1;
            }
            else
            {
                elemEscrito = 0;
            }
        }
        else if (tamanho == 1)
        {
            elemEscrito = AtualmenteEscrito;
        }
        else
        {
            elemEscrito = AtualmenteEscrito / tamanho;
        }
        stream->InicioBufferR += AtualmenteEscrito;
        return (elemEscrito);
    }
}

static void escreverDevagarArquivo(const void *ptr,
                       tamanho_t tamanho,
                       tamanho_t nmemb,
                       ARQUIVO *stream,
                       tamanho_t aEscrever,
                       tamanho_t *elemEscrito)
{
    tamanho_t AtualmenteEscrito;

    
    if (stream->ProximoLista == stream->fimbuffer)
    {
        stream->InicioBufferR += (stream->fimbuffer - stream->fbuf);
        stream->ProximoLista = stream->fbuf;
        stream->modo = __MODO_ESCRITA;
    }
    if ((stream->modoTexto) || (stream->TecnicaBuffer == _IOLBF))
    {
        escreverDevagarT(ptr, stream, aEscrever, &AtualmenteEscrito);
    }
    else
    {
        escreverDevagarB(ptr, stream, aEscrever, &AtualmenteEscrito);
    }
    if (nmemb == 1)
    {
        if (AtualmenteEscrito == tamanho)
        {
            *elemEscrito = 1;
        }
        else
        {
            *elemEscrito = 0;
        }
    }
    else if (tamanho == 1)
    {
        *elemEscrito = AtualmenteEscrito;
    }
    else
    {
        *elemEscrito = AtualmenteEscrito / tamanho;
    }
    return;
}



static void escreverDevagarT(const void *ptr,
                        ARQUIVO *stream,
                        tamanho_t aEscrever,
                        tamanho_t *AtualmenteEscrito)
{
    char *p;
    char *tptr;
    char *antigop;
    tamanho_t diffp;
    tamanho_t rem;
    int leme;

#ifdef __MSDOS__
    tamanho_t EscritaTemporaria;
    int errind;
#endif

    *AtualmenteEscrito = 0;
    tptr = (char *)ptr;
    p = tptr;
    antigop = p;
    p = (char *)memchr(antigop, '\n', aEscrever - (tamanho_t)(antigop - tptr));
    while (p != NULL)
    {
        diffp = (tamanho_t)(p - antigop);
        leme = 0;
        while (!leme)
        {
            rem = (tamanho_t)(stream->fimbuffer - stream->ProximoLista);
            if (diffp < rem)
            {
                memcpy(stream->ProximoLista, antigop, diffp);
                stream->ProximoLista += diffp;
                *AtualmenteEscrito += diffp;
                leme = 1;
            }
            else
            {
                memcpy(stream->ProximoLista, antigop, rem);
                antigop += rem;
                diffp -= rem;

#ifdef __MSDOS__
                EscritaTemporaria = __escrever(stream->ManipuladorArquivo,
                                      stream->fbuf,
                                      stream->tamanhoabuf,
                                      &errind);
                if (errind)
                {
                    stream->ErronoArquivo = 1;
                    return;
                }
#endif
                else
                {
                    *AtualmenteEscrito += rem;
                    stream->ProximoLista = stream->fbuf;
                    stream->InicioBufferR += EscritaTemporaria;
                }
            }
        }
        rem = (tamanho_t)(stream->fimbuffer - stream->ProximoLista);
        if (rem < 3)
        {

#ifdef __MSDOS__
            EscritaTemporaria = __escrever(stream->ManipuladorArquivo,
                                  stream->fbuf,
                                  (tamanho_t)(stream->ProximoLista - stream->fbuf),
                                  &errind);
            if (errind)
            {
                stream->ErronoArquivo = 1;
                errno = EscritaTemporaria;
                return;
            }
#endif
            stream->ProximoLista = stream->fbuf;
            stream->InicioBufferR += EscritaTemporaria;
        }
        if (stream->modoTexto)
        {
            memcpy(stream->ProximoLista, "\r\n", 2);
            stream->ProximoLista += 2;
        }
        else
        {
            memcpy(stream->ProximoLista, "\n", 1);
            stream->ProximoLista += 1;
        }
        *AtualmenteEscrito += 1;
        antigop = p + 1;
        p = (char *)memchr(antigop, '\n', aEscrever - (tamanho_t)(antigop - tptr));
    }

    if ((stream->TecnicaBuffer == _IOLBF)
        && (stream->ProximoLista != stream->fbuf)
        && (antigop != tptr))
    {

#ifdef __MSDOS__
        EscritaTemporaria = __escrever(stream->ManipuladorArquivo,
                              stream->fbuf,
                              (tamanho_t)(stream->ProximoLista - stream->fbuf),
                              &errind);
        if (errind)
        {
            stream->ErronoArquivo = 1;
            errno = EscritaTemporaria;
            return;
        }
#endif
        stream->ProximoLista = stream->fbuf;
        stream->InicioBufferR += EscritaTemporaria;
    }

    diffp = aEscrever - *AtualmenteEscrito;
    while (diffp != 0)
    {
        rem = (tamanho_t)(stream->fimbuffer - stream->ProximoLista);
        if (diffp < rem)
        {
            memcpy(stream->ProximoLista, antigop, diffp);
            stream->ProximoLista += diffp;
            *AtualmenteEscrito += diffp;
        }
        else
        {
            memcpy(stream->ProximoLista, antigop, rem);

#ifdef __MSDOS__
            EscritaTemporaria = __escrever(stream->ManipuladorArquivo,
                                  stream->fbuf,
                                  stream->tamanhoabuf,
                                  &errind);
            if (errind)
            {
                stream->ErronoArquivo = 1;
                errno = EscritaTemporaria;
                return;
            }
#endif
            else
            {
                *AtualmenteEscrito += rem;
                stream->ProximoLista = stream->fbuf;
            }
            stream->InicioBufferR += EscritaTemporaria;
            antigop += rem;
        }
        diffp = aEscrever - *AtualmenteEscrito;
    }
    if ((stream->TecnicaBuffer == _IONBF)
        && (stream->ProximoLista != stream->fbuf))
    {

#ifdef __MSDOS__
        EscritaTemporaria = __escrever(stream->ManipuladorArquivo,
                              stream->fbuf,
                              (tamanho_t)(stream->ProximoLista - stream->fbuf),
                              &errind);
        if (errind)
        {
            stream->ErronoArquivo = 1;
            errno = EscritaTemporaria;
            return;
        }
#endif
        stream->ProximoLista = stream->fbuf;
        stream->InicioBufferR += EscritaTemporaria;
    }
    return;
}


static void escreverDevagarB(const void *ptr,
                        ARQUIVO *stream,
                        tamanho_t aEscrever,
                        tamanho_t *AtualmenteEscrito)
{
    tamanho_t dispensar;

#ifdef __MSDOS__
    tamanho_t EscritaTemporaria;
    int errind;
#endif

    dispensar = (tamanho_t)(stream->fimbuffer - stream->ProximoLista);
    if (aEscrever < dispensar)
    {
        memcpy(stream->ProximoLista, ptr, aEscrever);
        *AtualmenteEscrito = aEscrever;
        stream->ProximoLista += aEscrever;
        return;
    }
    memcpy(stream->ProximoLista, ptr, dispensar);

#ifdef __MSDOS__
    EscritaTemporaria = __escrever(stream->ManipuladorArquivo,
                          stream->fbuf,
                          stream->tamanhoabuf,
                          &errind);
    if (errind)
    {
        stream->ErronoArquivo = 1;
        errno = EscritaTemporaria;
        return;
    }
#endif
    *AtualmenteEscrito = dispensar;
    stream->ProximoLista = stream->fbuf;
    stream->InicioBufferR += EscritaTemporaria;
    if (aEscrever > stream->tamanhoabuf)
    {
        stream->BinRapido = 1;

#ifdef __MSDOS__
        EscritaTemporaria = __escrever(stream->ManipuladorArquivo,
                              (char *)ptr + *AtualmenteEscrito,
                              aEscrever - *AtualmenteEscrito,
                              &errind);
        if (errind)
        {
            stream->ErronoArquivo = 1;
            errno = EscritaTemporaria;
            return;
        }
#endif
        *AtualmenteEscrito += EscritaTemporaria;
        stream->InicioBufferR += EscritaTemporaria;
    }
    else
    {
        memcpy(stream->fbuf,
               (char *)ptr + *AtualmenteEscrito,
               aEscrever - *AtualmenteEscrito);
        stream->ProximoLista += (aEscrever - *AtualmenteEscrito);
        *AtualmenteEscrito = aEscrever;
    }
    stream->InicioBufferR += *AtualmenteEscrito;
    return;
}
#endif

static int vvprintf(const char *formato_STDIO, va_list arg, ARQUIVO *fq, char *s)
{
    int leme = 0;
    int vint;
    double vdbl;
    unsigned int uvint;
    char *vcptr;
    int contador_chars = 0;
    tamanho_t tamanho;
    char numbuf[50];
    char *nptr;

    while (!leme)
    {
        if (*formato_STDIO == '\0')
        {
            leme = 1;
        }
        else if (*formato_STDIO == '%')
        {
            formato_STDIO++;
            if (*formato_STDIO == 'd')
            {
                vint = va_arg(arg, int);
                if (vint < 0)
                {
                    uvint = -vint;
                }
                else
                {
                    uvint = vint;
                }
                nptr = numbuf;
                do
                {
                    *nptr++ = (char)('0' + uvint % 10);
                    uvint /= 10;
                } while (uvint > 0);
                if (vint < 0)
                {
                    *nptr++ = '-';
                }
                do
                {
                    nptr--;
                    outch(*nptr);
                    contador_chars++;
                } while (nptr != numbuf);
            }
            else if (strchr("eEgGfF", *formato_STDIO) != NULL && *formato_STDIO != 0)
            {
                vdbl = va_arg(arg, double);
                dblcvt(vdbl, *formato_STDIO, 0, 6, numbuf);   /* 'e','f' etc. */
                tamanho = tamanhostring(numbuf);
                if (fq == NULL)
                {
                    memcpy(s, numbuf, tamanho);
                    s += tamanho;
                }
                else
                {
                    fputs(numbuf, fq);
                }
                contador_chars += tamanho;
            }
            else if (*formato_STDIO == 's')
            {
                vcptr = va_arg(arg, char *);
                if (vcptr == NULL)
                {
                    vcptr = "(vazio)";
                }
                if (fq == NULL)
                {
                    tamanho = tamanhostring(vcptr);
                    memcpy(s, vcptr, tamanho);
                    s += tamanho;
                    contador_chars += tamanho;
                }
                else
                {
                    fputs(vcptr, fq);
                    contador_chars += tamanhostring(vcptr);
                }
            }
            else if (*formato_STDIO == 'c')
            {
                vint = va_arg(arg, int);
                outch(vint);
                contador_chars++;
            }
            else if (*formato_STDIO == '%')
            {
                outch('%');
                contador_chars++;
            }
            else
            {
                int extraCh;

                extraCh = examine(&formato_STDIO, fq, s, &arg, contador_chars);
                contador_chars += extraCh;
                if (s != NULL)
                {
                    s += extraCh;
                }
            }
        }
        else
        {
            outch(*formato_STDIO);
            contador_chars++;
        }
        formato_STDIO++;
    }
    return (contador_chars);
}

static int examine(const char **formt, ARQUIVO *fq, char *s, va_list *arg,
                   int contador_chars)
{
    int extraCh = 0;
    int BandeiraMenos = 0;
    int BandeiraMais = 0;
    int BandeiraEspaco = 0;
    int BandeiraJogodaVelha = 0;
    int BandeiraZero = 0;
    int largura = 0;
    int precisao = -1;
    int metade = 0;
    int lng = 0;
    int especificador = 0;
    int leme;
    long lvalor;
    short int hvalor;
    int ivalor;
    unsigned long ulvalor;
    double vdbl;
    char *svalor;
    char trabalho[50];
    int x;
    int y;
    int rem;
    const char *formato_STDIO;
    int base;
    int PreencherChar;
    int neg;
    int comprimento;
    tamanho_t stamanho;

    unused(contador_chars);
    formato_STDIO = *formt;
    
    leme = 0;
    while (!leme)
    {
        switch (*formato_STDIO)
        {
            case '-': BandeiraMenos = 1;
                      break;
            case '+': BandeiraMais = 1;
                      break;
            case ' ': BandeiraEspaco = 1;
                      break;
            case '#': BandeiraJogodaVelha = 1;
                      break;
            case '0': BandeiraZero = 1;
                      break;
            case '*': largura = va_arg(*arg, int);
                      break;
            default:  leme = 1;
                      break;
        }
        if (!leme)
        {
            formato_STDIO++;
        }
        else
        {
            if (BandeiraEspaco && BandeiraMais)
            {
                BandeiraEspaco = 0;
            }
            if (BandeiraMenos)
            {
                BandeiraZero = 0;
            }
        }
    }

    /* processando largura */
    if (eumdigito((unsigned char)*formato_STDIO))
    {
        while (eumdigito((unsigned char)*formato_STDIO))
        {
            largura = largura * 10 + (*formato_STDIO - '0');
            formato_STDIO++;
        }
    }

    /* processando precisão */
	
    if (*formato_STDIO == '.')
    {
        formato_STDIO++;
        if (*formato_STDIO == '*')
        {
            precisao = va_arg(*arg, int);
            formato_STDIO++;
        }
        else
        {
            precisao = 0;
            while (eumdigito((unsigned char)*formato_STDIO))
            {
                precisao = precisao * 10 + (*formato_STDIO - '0');
                formato_STDIO++;
            }
        }
    }

   
    if (*formato_STDIO == 'h')
    {
        
    }
    else if (*formato_STDIO == 'l')
    {
        lng = 1;
    }
    else if (*formato_STDIO == 'L')
    {
        lng = 1;
    }
    else
    {
        formato_STDIO--;
    }
    formato_STDIO++;

    
    especificador = *formato_STDIO;

    if (strchr("dxXuiop", especificador) != NULL && especificador != 0)
    {
        if (precisao < 0)
        {
            precisao = 1;
        }
#if defined(__MSDOS__) && !defined(__PXDOS__)
        if (especificador == 'p')
        {
            lng = 1;
        }
#endif
        if (lng)
        {
            lvalor = va_arg(*arg, long);
        }
        else if (metade)
        {
            hvalor = va_arg(*arg, short);
            if (especificador == 'u') lvalor = (unsigned short)hvalor;
            else lvalor = hvalor;
        }
        else
        {
            ivalor = va_arg(*arg, int);
            if (especificador == 'u') lvalor = (unsigned int)ivalor;
            else lvalor = ivalor;
        }
        ulvalor = (unsigned long)lvalor;
        if ((lvalor < 0) && ((especificador == 'd') || (especificador == 'i')))
        {
            neg = 1;
            ulvalor = -lvalor;
        }
        else
        {
            neg = 0;
        }
#if defined(__MSDOS__)
        if (!lng)
        {
            ulvalor &= 0xffff;
        }
#endif
        if ((especificador == 'X') || (especificador == 'x') || (especificador == 'p'))
        {
            base = 16;
        }
        else if (especificador == 'o')
        {
            base = 8;
        }
        else
        {
            base = 10;
        }
        if (especificador == 'p')
        {
#if defined(__OS2__) || defined(__PXDOS__) || defined(__WIN32__)
            precisao = 8;
#endif
#if defined(__MSDOS__) && !defined(__PXDOS__)
            precisao = 9;
#endif
        }
        x = 0;
        while (ulvalor > 0)
        {
            rem = (int)(ulvalor % base);
            if (rem < 10)
            {
                trabalho[x] = (char)('0' + rem);
            }
            else
            {
                if ((especificador == 'X') || (especificador == 'p'))
                {
                    trabalho[x] = (char)('A' + (rem - 10));
                }
                else
                {
                    trabalho[x] = (char)('a' + (rem - 10));
                }
            }
            x++;
#if defined(__MSDOS__) && !defined(__PXDOS__)
            if ((x == 4) && (especificador == 'p'))
            {
                trabalho[x] = ':';
                x++;
            }
#endif
            ulvalor = ulvalor / base;
        }
#if defined(__MSDOS__) && !defined(__PXDOS__)
        if (especificador == 'p')
        {
            while (x < 5)
            {
                trabalho[x] = (x == 4) ? ':' : '0';
                x++;
            }
        }
#endif
        while (x < precisao)
        {
            trabalho[x] = '0';
            x++;
        }
        if (neg)
        {
            trabalho[x++] = '-';
        }
        if (BandeiraZero)
        {
            PreencherChar = '0';
        }
        else
        {
            PreencherChar = ' ';
        }
        y = x;
        if (!BandeiraMenos)
        {
            while (y < largura)
            {
                outch(PreencherChar);
                extraCh++;
                y++;
            }
        }
        if (BandeiraJogodaVelha && (paramaiusculo((unsigned char)especificador) == 'X'))
        {
            outch('0');
            outch('x');
            extraCh += 2;
        }
        x--;
        while (x >= 0)
        {
            outch(trabalho[x]);
            extraCh++;
            x--;
        }
        if (BandeiraMenos)
        {
            while (y < largura)
            {
                outch(PreencherChar);
                extraCh++;
                y++;
            }
        }
    }
    else if (strchr("eEgGfF", especificador) != NULL && especificador != 0)
    {
        if (precisao < 0)
        {
            precisao = 6;
        }
        vdbl = va_arg(*arg, double);
        dblcvt(vdbl, especificador, largura, precisao, trabalho);   /* 'e','f' etc. */
        stamanho = tamanhostring(trabalho);
        if (fq == NULL)
        {
            memcpy(s, trabalho, stamanho);
            s += stamanho;
        }
        else
        {
            fputs(trabalho, fq);
        }
        extraCh += stamanho;
    }
    else if (especificador == 's')
    {
        if (precisao < 0)
        {
            precisao = 1;
        }
        svalor = va_arg(*arg, char *);
        PreencherChar = ' ';
        if (precisao > 1)
        {
            char *p;

            p = memchr(svalor, '\0', precisao);
            if (p != NULL)
            {
                comprimento = (int)(p - svalor);
            }
            else
            {
                comprimento = precisao;
            }
        }
        else
        {
            comprimento = tamanhostring(svalor);
        }
        if (!BandeiraMenos)
        {
            if (comprimento < largura)
            {
                extraCh += (largura - comprimento);
                for (x = 0; x < (largura - comprimento); x++)
                {
                    outch(PreencherChar);
                }
            }
        }
        for (x = 0; x < comprimento; x++)
        {
            outch(svalor[x]);
        }
        extraCh += comprimento;
        if (BandeiraMenos)
        {
            if (comprimento < largura)
            {
                extraCh += (largura - comprimento);
                for (x = 0; x < (largura - comprimento); x++)
                {
                    outch(PreencherChar);
                }
            }
        }
    }
    *formt = formato_STDIO;
    return (extraCh);
}

int fputc(int c, ARQUIVO *stream)
{
    char buf[1];

#if !defined(__MVS__) && !defined(__CMS__)
    stream->BinRapido = 0;
    if ((stream->ProximoLista < (stream->fimbuffer - 2))
        && (stream->TecnicaBuffer != _IONBF))
    {
        if (stream->modoTexto)
        {
            if (c == '\n')
            {
                if (stream->TecnicaBuffer == _IOFBF)
                {
                    *stream->ProximoLista++ = '\r';
                    *stream->ProximoLista++ = '\n';
                }
                else
                {
                    buf[0] = (char)c;
                    if (escArquivo(buf, 1, 1, stream) != 1)
                    {
                        return (EOF);
                    }
                }
            }
            else
            {
                *stream->ProximoLista++ = (char)c;
            }
        }
        else
        {
            *stream->ProximoLista++ = (char)c;
        }
    }
    else
#endif
    {
        buf[0] = (char)c;
        if (escArquivo(buf, 1, 1, stream) != 1)
        {
            return (EOF);
        }
    }
    return (c);
}

#if !defined(__MVS__) && !defined(__CMS__)
int fputs(const char *s, ARQUIVO *stream)
{
    tamanho_t tamanho;
    tamanho_t ret;

    tamanho = tamanhostring(s);
    ret = escArquivo(s, tamanho, 1, stream);
    if (ret != 1) return (EOF);
    else return (0);
}
#endif

int remover(const char *nome_de_arquivo)
{
    int ret;



#ifdef __MSDOS__
    __remover(nome_de_arquivo);
    ret = 0;
#endif
    return (ret);
}

int renomear(const char *antigo, const char *new)
{
    int ret;

#ifdef __MSDOS__
    __renomear(antigo, new);
    ret = 0;
#endif
    return (ret);
}

int sprintf(char *s, const char *formato_STDIO, ...)
{
    va_list arg;
    int ret;

    va_start(arg, formato_STDIO);
    ret = vsprintf(s, formato_STDIO, arg);
    va_end(arg);
    return (ret);
}

int vsprintf(char *s, const char *formato_STDIO, va_list arg)
{
    int ret;

    ret = vvprintf(formato_STDIO, arg, NULL, s);
    if (ret >= 0)
    {
        *(s + ret) = '\0';
    }
    return (ret);
}


#if !defined(__MVS__) && !defined(__CMS__)
char *fgets(char *s, int n, ARQUIVO *stream)
{
    char *p;
    register char *t;
    register char *u = s;
    int c;
    int processado;

#ifdef __MSDOS__
    tamanho_t LeituraAtual;
    int errind;
#endif

    if (stream->TextoRapido)
    {
        p = stream->ProximoLista + n - 1;
        t = stream->ProximoLista;
        if (p < stream->fimbuffer)
        {
            c = *p;
            *p = '\n';

                while ((*u++ = *t++) != '\n') ; 

            *p = (char)c;
            if (t <= p)
            {
                if (*(t - 2) == '\r') /* t is protected, u isn't */
                {
                    *(u - 2) = '\n';
                    *(u - 1) = '\0';
                }
                else
                {
                    *u = '\0';
                }
                stream->ProximoLista = t;
                return (s);
            }
            else
            {
                processado = (int)(t - stream->ProximoLista) - 1;
                stream->ProximoLista = t - 1;
                u--;
            }
        }
        else
        {
            while ((*u++ = *t++) != '\n') ; 
            if (t <= stream->fimbuffer)
            {
                if (*(t - 2) == '\r') 
                {
                    *(u - 2) = '\n';
                    *(u - 1) = '\0';
                }
                else
                {
                    *u = '\0';
                }
                stream->ProximoLista = t;
                return (s);
            }
            else
            {
                processado = (int)(t - stream->ProximoLista) - 1;
                stream->ProximoLista = t - 1;
                u--;
            }
        }
    }
    else
    {
        processado = 0;
    }

    if (n < 1)
    {
        return (NULL);
    }
    if (n < 2)
    {
        *u = '\0';
        return (s);
    }
    if (stream->ungetCh != -1)
    {
        processado++;
        *u++ = (char)stream->ungetCh;
        stream->ungetCh = -1;
    }
    while (1)
    {
        t = stream->ProximoLista;
        p = stream->ProximoLista + (n - processado) - 1;
        if (p < stream->fimbuffer)
        {
            c = *p;
            *p = '\n';
        }
        if (stream->noNl)
        {
            while (((*u++ = *t) != '\n') && (*t++ != '\r')) ;
            if (*(u - 1) == '\n')
            {
                t++;
            }
            else
            {
                u--;
                while ((*u++ = *t++) != '\n') ;
            }
        }
        else
        {
            while ((*u++ = *t++) != '\n') ; /* tight inner loop */
        }
        if (p < stream->fimbuffer)
        {
            *p = (char)c;
        }
        if (((t <= p) && (p < stream->fimbuffer))
           || ((t <= stream->fimbuffer) && (p >= stream->fimbuffer)))
        {
            if (stream->modoTexto)
            {
                if (stream->noNl)
                {
                    if ((*(t - 1) == '\r') || (*(t - 1) == '\n'))
                    {
                        *(u - 1) = '\0';
                    }
                    else
                    {
                        *u = '\0';
                    }
                }
                else if (*(t - 2) == '\r') /* t é protegido, u não */
                {
                    *(u - 2) = '\n';
                    *(u - 1) = '\0';
                }
                else
                {
                    *u = '\0';
                }
            }
            stream->ProximoLista = t;
            if (stream->modoTexto)
            {
                stream->TextoRapido = 1;
            }
            return (s);
        }
        else if (((t > p) && (p < stream->fimbuffer))
                 || ((t > stream->fimbuffer) && (p >= stream->fimbuffer)))
        {
            int deixar = 1;

            if (stream->modoTexto)
            {
                if (t > stream->fimbuffer)
                {
                    if ((t - stream->ProximoLista) > 1)
                    {
                        if (*(t - 2) == '\r') /* t é protegido, u não */
                        {
                            processado -= 1; /* preparar para adicionar */
                        }
                    }
                    deixar = 0;
                }
                else
                {
                    if ((*(t - 2) == '\r') && (*(t - 1) == '\n'))
                    {
                        *(u - 2) = '\n';
                        *(u - 1) = '\0';
                    }
                    else
                    {
                        t--;
                        *(u - 1) = '\0';
                    }
                }
            }
            else if (t > stream->fimbuffer)
            {
                deixar = 0;
            }
            else
            {
                *u = '\0';
            }
            if (deixar)
            {
                stream->ProximoLista = t;
                if (stream->modoTexto)
                {
                    stream->TextoRapido = 1;
                }
                return (s);
            }
        }
        processado += (int)(t - stream->ProximoLista) - 1;
        u--;
        stream->InicioBufferR += (stream->fimbuffer - stream->fbuf);


#ifdef __MSDOS__
        LeituraAtual = __ler(stream->ManipuladorArquivo,
                            stream->fbuf,
                            stream->tamanhoabuf,
                            &errind);
        if (errind)
        {
            errno = LeituraAtual;
            LeituraAtual = 0;
            stream->ErronoArquivo = 1;
        }
#endif
        stream->fimbuffer = stream->fbuf + LeituraAtual;
        *stream->fimbuffer = '\n';
        if (LeituraAtual == 0)
        {
            *u = '\0';
            if ((u - s) <= 1)
            {
                stream->eofInd = 1;
                return (NULL);
            }
            else
            {
                return (s);
            }
        }
        stream->ProximoLista = stream->fbuf;
    }
}
#endif

int ungetc(int c, ARQUIVO *stream)
{
    if ((stream->ungetCh != -1) || (c == EOF))
    {
        return (EOF);
    }
    stream->ungetCh = (unsigned char)c;
    stream->TextoRapido = 0;
    stream->BinRapido = 0;
    return ((unsigned char)c);
}

int fgetc(ARQUIVO *stream)
{
    unsigned char x[1];
    tamanho_t ret;

    ret = ArquivoLer(x, 1, 1, stream);
    if (ret == 0)
    {
        return (EOF);
    }
    return ((int)x[0]);
}

int fobter(ARQUIVO *stream, long int offset, int deOnde)
{
    long posicaoantiga;
    long posicaonova;


    posicaoantiga = stream->InicioBufferR + (stream->ProximoLista - stream->fbuf);
    if (stream->modo == __MODO_ESCRITA)
    {
        fflush(stream);
    }
    if (deOnde == DEFINIR_OBTER)
    {
        posicaonova = offset;
    }
    else if (deOnde == OBTER_COR)
    {
        posicaonova = posicaoantiga + offset;
    }

    if (deOnde == OBTER_FIM)
    {
        char buf[1000];

        while (ArquivoLer(buf, sizeof buf, 1, stream) == 1)
        {
           // Não fazer nada
        }
    }
    else if ((posicaonova >= stream->InicioBufferR)
        && (posicaonova < (stream->InicioBufferR + (stream->fimbuffer - stream->fbuf)))
        && (stream->atualizar || (stream->modo == __MODO_LEITURA)))
    {
        stream->ProximoLista = stream->fbuf + (tamanho_t)(posicaonova - stream->InicioBufferR);
    }
    else
    {


#ifdef __MSDOS__
        __obter(stream->ManipuladorArquivo, posicaonova, deOnde);
        stream->fimbuffer = stream->fbuf + stream->tamanhoabuf;
        stream->ProximoLista = stream->fimbuffer;
        stream->InicioBufferR = posicaonova - stream->tamanhoabuf;
#endif


    }
    stream->BinRapido = 0;
    stream->TextoRapido = 0;
    stream->ungetCh = -1;
    return (0);
}

long int ftell(ARQUIVO *stream)
{
    return (stream->InicioBufferR + (stream->ProximoLista - stream->fbuf));
}

int defposicaoarquivo(ARQUIVO *stream, const fpos_t *pos)
{
    fobter(stream, *pos, DEFINIR_OBTER);
    return (0);
}

int obtposicaoarquivo(ARQUIVO *stream, fpos_t *pos)
{
    *pos = ftell(stream);
    return (0);
}

void rewind(ARQUIVO *stream)
{
    fobter(stream, 0L, DEFINIR_OBTER);
    return;
}

void limparErro(ARQUIVO *stream)
{
    stream->ErronoArquivo = 0;
    stream->eofInd = 0;
    return;
}

void manipularErro(const char *s)
{
    if ((s != NULL) && (*s != '\0'))
    {
        printf("%s: ", s);
    }
    if (errno == 0)
    {
        printf("Nenhum erro ocorreu.\n");
    }
    else
    {
        printf("Um erro ocorreu.\n");
    }
    return;
}

/*
NULL + F = alocar, setup
NULL + L = alocar, setup
NULL + N = ignorar, retornar sucesso
buf  + F = setup
buf  + L = setup
buf  + N = ignorar, retornar sucesso
*/

int setvbuf(ARQUIVO *stream, char *buf, int modo, tamanho_t tamanho)
{
    char *meuBuffer;


    if (modo == _IONBF)
    {
        stream->TecnicaBuffer = modo;
        return (0);
    }
    if (buf == NULL)
    {
        if (tamanho < 2)
        {
            return (-1);
        }
        meuBuffer = malloc(tamanho + 8);
        if (meuBuffer == NULL)
        {
            return (-1);
        }
    }
    else
    {
        if (tamanho < 10)
        {
            return (-1);
        }
        meuBuffer = buf;
        stream->EsteBuffer = 1;
        tamanho -= 8;
    }
    if (!stream->arquivoPermanente)
    {
        livre(stream->intBuffer);
    }
    stream->intBuffer = meuBuffer;
    stream->fbuf = stream->intBuffer + 2;
    *stream->fbuf++ = '\0';
    *stream->fbuf++ = '\0';
    stream->tamanhoabuf = tamanho;
    stream->fimbuffer = stream->fbuf + stream->tamanhoabuf;
    *stream->fimbuffer = '\n';
    if (stream->modo == __MODO_ESCRITA)
    {
        stream->ProximoLista = stream->fbuf;
    }
    else
    {
        stream->ProximoLista = stream->fimbuffer;
    }
    stream->TecnicaBuffer = modo;
    if (!stream->modoTexto && (stream->TecnicaBuffer == _IOLBF))
    {
        stream->BinRapido = 0;
    }
    return (0);
}

int definirBuffer(ARQUIVO *stream, char *buf)
{
    int ret;

    if (buf == NULL)
    {
        ret = setvbuf(stream, NULL, _IONBF, 0);
    }
    else
    {
        ret = setvbuf(stream, buf, _IOFBF, TamanhoBuffer_DEFINE);
    }
    return (ret);
}

ARQUIVO *freopen(const char *nome_de_arquivo, const char *modo, ARQUIVO *stream)
{
    int perm;

    perm = stream->arquivoPermanente;
    stream->arquivoPermanente = 1;
    fechar(stream);

    meuArquivo = stream;
    nome_arquivo_lib = nome_de_arquivo;
    modus = modo;
    err = 0;
    LocalSobressalente = stream->Numero_Arquivo_Interno;
    abrir2();
    if (err && !perm)
    {
        __Arquivos_Usuario[stream->Numero_Arquivo_Interno] = NULL;
        livre(stream);
    }
    stream->arquivoPermanente = perm;
    if (err)
    {
        return (NULL);
    }
    return (stream);
}

int fflush(ARQUIVO *stream)
{
#if !defined(__MVS__) && !defined(__CMS__)

#ifdef __MSDOS__
    int errind;
    tamanho_t AtualmenteEscrito;
#endif

    if ((stream->ProximoLista != stream->fbuf) && (stream->modo == __MODO_ESCRITA))
    {

#ifdef __MSDOS__
        AtualmenteEscrito = __escrever(stream->ManipuladorArquivo,
                                stream->fbuf,
                                (tamanho_t)(stream->ProximoLista - stream->fbuf),
                                &errind);
        if (errind)
        {
            stream->ErronoArquivo = 1;
            errno = AtualmenteEscrito;
            return (EOF);
        }
#endif
        stream->InicioBufferR += AtualmenteEscrito;
        stream->ProximoLista = stream->fbuf;
    }
#endif
    return (0);
}

char *NomeTemporarioDOS(char *s)
{

    static char buf[] = "ZZZZZZZA.$$$";

#if defined(__MVS__) || defined(__CMS__)
    buf[10]++;
#else
    buf[7]++;
#endif
    if (s == NULL)
    {
        return (buf);
    }
    strcpy(s, buf);
    return (s);
}

ARQUIVO *arquivoTemporarioDOS(void)
{

    return (abrir("ZZZZZZZA.$$$", "wb+"));

}

int fscanf(ARQUIVO *stream, const char *formato_STDIO, ...)
{
    va_list arg;
    int ret;

    va_start(arg, formato_STDIO);
    ret = vvscanf(formato_STDIO, arg, stream, NULL);
    va_end(arg);
    return (ret);
}

int scanf(const char *formato_STDIO, ...)
{
    va_list arg;
    int ret;

    va_start(arg, formato_STDIO);
    ret = vvscanf(formato_STDIO, arg, stdin, NULL);
    va_end(arg);
    return (ret);
}

int sscanf(const char *s, const char *formato_STDIO, ...)
{
    va_list arg;
    int ret;

    va_start(arg, formato_STDIO);
    ret = vvscanf(formato_STDIO, arg, NULL, s);
    va_end(arg);
    return (ret);
}


static int vvscanf(const char *formato_STDIO, va_list arg, ARQUIVO *fp, const char *s)
{
    int ch;
    int leme = 0;
    int cnt = 0;
    char *cptr;
    int *iptr;
    unsigned int *uptr;
    long *lptr;
    unsigned long *luptr;
    short *hptr;
    unsigned short *huptr;
    double *dptr;
    float *fptr;
    int modlong;   
    int modshort;   
    int Item_Informacao; 
    int prevwasc;  

    inch();
    Item_Informacao = 0;   /* init */
    prevwasc = 0;
	
    if ((fp != NULL && ch == EOF) || (fp == NULL && ch == 0)) return EOF;
                               /* Iniciar com EOF no fim da String */
    while (!leme)
    {
        if (*formato_STDIO == '\0')
        {
            leme = 1;
        }
        else if (*formato_STDIO == '%' || Item_Informacao)
        {
            if(*formato_STDIO=='%')   /* Iniciando um Item */
            {
                formato_STDIO++;
                modlong=0;   /* init */
                modshort=0;
            }
            if (*formato_STDIO == '%')   /* %% */
            {
                if (prevwasc) inch();
                prevwasc = 0;
                if (ch != '%') return (cnt);
                inch();
                Item_Informacao=0;
            }
            else if (*formato_STDIO == 'l')
            {
                /* Modificador: l  (%ld ou %lf) */
                modlong=1;
                Item_Informacao=1;
            }
            else if (*formato_STDIO == 'h')
            {
                /* Modificador: h (short int) */
                modshort=1;
                Item_Informacao=1;
            }
            else    /* Processador um caractere: */
            {
                Item_Informacao=0;  
                if (*formato_STDIO == 's')
                {
                    if (prevwasc) inch();
                    prevwasc = 0;
                    cptr = va_arg(arg, char *);
                    /* Pular espaçoes em branco: */
                    while (ch>=0 && eespaco(ch)) inch();
                    if ((fp != NULL && ch == EOF) || (fp == NULL && ch == 0))
                                         
                    {
                        leme = 1;
                        *cptr = 0;   
                    }
                    else
                    {
                        for(;;)
                        {
                            if (eespaco(ch)) break;
                            if ((fp != NULL && ch == EOF)
                                || (fp == NULL && ch == 0))
                            {
                                leme = 1;
                                break;
                            }
                            *cptr++ = (char)ch;
                            inch();
                        }
                        *cptr = '\0';
                        cnt++;
                    }
                }
                else if (*formato_STDIO == 'c')
                {
                    if (prevwasc) inch();
                       
                    prevwasc = 1;
                    cptr = va_arg(arg, char *);
                    if ((fp != NULL && ch == EOF)
                        || (fp == NULL && ch == 0)) leme = 1;
                                          /* at EOF or end of string */
                    else
                    {
                        *cptr = ch;
                        cnt++;
                    }
                }
                else if (*formato_STDIO == 'd' || *formato_STDIO == 'u')
                {
                    int neg = 0;
                    long lval;

                    if (prevwasc) inch();
                    prevwasc = 0;
                    if (*formato_STDIO != 'u')
                    {
                        if (modlong) lptr = va_arg(arg, long *);
                        else if (modshort) hptr = va_arg(arg, short *);
                        else iptr = va_arg(arg, int *);
                    }
                    else
                    {
                        if (modlong) luptr = va_arg(arg, unsigned long *);
                        else if (modshort) huptr =
                                 va_arg(arg, unsigned short *);
                        else uptr = va_arg(arg, unsigned int *);
                    }
                   
                    while (ch>=0 && eespaco(ch)) inch();
                    if (ch == '-')
                    {
                        neg = 1;
                        inch();
                    }
                    else if(ch == '+') inch();
                    if (!eumdigito((unsigned char)ch)) return (cnt);
                    lval = ch - '0';
                    inch();
                    while ((ch >= 0) && (eumdigito(ch)))
                    {
                        lval = lval * 10 + (ch - '0');
                        inch();
                    }
                    if ((fp != NULL && ch == EOF)
                        || (fp == NULL && ch == 0)) leme = 1;
                    if (neg)
                    {
                        lval = -lval;
                    }
                    if (*formato_STDIO != 'u')
                    {
                        if (modlong) *lptr=lval;
                    
                        else if (modshort) *hptr = (short)lval;
                          
                        else *iptr=(int)lval;
                    }
                    else
                    {
                        if (modlong) *luptr = (unsigned long)lval;
                        else if (modshort) *huptr = (unsigned short)lval;
                        else *uptr = (unsigned int)lval;
                    }
                    cnt++;
                }
                else if (*formato_STDIO=='e' || *formato_STDIO=='f' || *formato_STDIO=='g' ||
                         *formato_STDIO=='E' || *formato_STDIO=='G')
                {
                   
                    int negsw1,negsw2,pontosw,expsw,ndigs1,ndigs2,nfdigs;
                    int ntrailzer,expnum,sinalExpressaosw;
                    double fpval,pow10;

                    if (prevwasc) inch();
                    prevwasc = 0;
                    if (modlong) dptr = va_arg(arg, double *);
                    else fptr = va_arg(arg, float *);
                    negsw1=0;   
                    negsw2=0;
                    pontosw=0;
                    expsw=0;
                    ndigs1=0;
                    ndigs2=0;
                    nfdigs=0;
                    ntrailzer=0; 
                    expnum=0;
                    sinalExpressaosw=0;  
                    fpval=0.0;
                   
                    while (ch>=0 && eespaco(ch)) inch();
                    if (ch=='-')
                    {
                        negsw1=1;
                        inch();
                    }
                    else if (ch=='+') inch();
                    while (ch>0)
                    {
                        if (ch=='.' && pontosw==0 && expsw==0) pontosw=1;
                        else if (eumdigito(ch))
                        {
                            if(expsw)
                            {
                                ndigs2++;
                                expnum=expnum*10+(ch-'0');
                            }
                            else
                            {
                                
                                ndigs1++;
                                if (pontosw) nfdigs++;
                                if (ch=='0' && fpval!=0.)
                                {
                                  
                                    ntrailzer++;
                                }
                                else
                                {
                                    
                                    while (ntrailzer>0)
                                    {
                                        fpval*=10.;
                                        ntrailzer--;
                                    }
                                    fpval=fpval*10.0+(ch-'0');
                                }
                            }
                        }
                        else if ((ch=='e' || ch=='E') && expsw==0) expsw=1;
                        else if ((ch=='+' || ch=='-') && expsw==1
                                 && ndigs2==0 && sinalExpressaosw==0)
                        {
                            sinalExpressaosw=1;
                            if (ch=='-') negsw2=1;
                        }
                        else break;   
                        inch();
                    }
                    if ((fp != NULL && ch == EOF)
                        || (fp == NULL && ch == 0)) leme=1;
                   
                    if (ndigs1==0 || (expsw && ndigs2==0)) return(cnt);
                 
                    if (negsw2) expnum=-expnum;
                    expnum+=ntrailzer-nfdigs;
                    if (expnum!=0 && fpval!=0.0)
                    {
                        negsw2=0;
                        if (expnum<0)
                        {
                            expnum=-expnum;
                            negsw2=1;
                        }
                       
                        pow10=10.0;
                        for (;;)
                        {
                            if (expnum & 1)    
                            {
                                if (negsw2) fpval/=pow10;
                                else fpval*=pow10;
                            }
                            expnum>>=1;  
                            if (expnum==0) break;
                            pow10*=pow10; 
                        }
                    }
                    if (negsw1) fpval=-fpval;
                    if (modlong) *dptr=fpval; 
                    else *fptr=(float)fpval;
                    cnt++;
                }
            }
        }
        else if (eespaco((unsigned char)(*formato_STDIO)))
        {
           
            if (prevwasc) inch();
            prevwasc = 0;
            while (ch>=0 && eespaco(ch))
            {
                inch();
            }
        }
        else  
        {
            if (prevwasc) inch();
            prevwasc = 0;
            if (ch != *formato_STDIO) return (cnt);
            inch();
        }
        formato_STDIO++;
        if ((fp != NULL && ch == EOF) || (fp == NULL && ch == 0)) leme = 1;
            /* EOF */
    }
    return (cnt);
}

char *gets(char *s)
{
    char *ret;

    stdin->TextoRapido = 0;
    stdin->noNl = 1;
    ret = fgets(s, INT_MAX, stdin);
    stdin->noNl = 0;
    stdin->TextoRapido = 1;
    return (ret);
}

int puts(const char *s)
{
    int ret;

    ret = fputs(s, stdout);
    if (ret == EOF)
    {
        return (ret);
    }
    return (putc('\n', stdout));
}



#undef getchar
#undef putchar
#undef getc
#undef putc
#undef feof
#undef ferror

int getc(ARQUIVO *stream)
{
    return (fgetc(stream));
}

int putc(int c, ARQUIVO *stream)
{
    return (fputc(c, stream));
}

int getchar(void)
{
    return (getc(stdin));
}

int putchar(int c)
{
    return (putc(c, stdout));
}

int feof(ARQUIVO *stream)
{
    return (stream->eofInd);
}

int ferror(ARQUIVO *stream)
{
    return (stream->ErronoArquivo);
}




static void dblcvt(double num, char cnvtype, tamanho_t nlargura,
            tamanho_t nprecisao, char *resultado)
{
    double b,round;
    int i,j,exp,pdigitos,formato_STDIO;
    char sinalSTDIO, trabalho[45];



    if ( num < 0 )
    {
        b = -num;
        sinalSTDIO = '-';
    }
    else
    {
        b = num;
        sinalSTDIO = ' ';
    }

   

    exp = 0;
    if( b > 1.0 )
    {
        while(b >= 10.0)
        {
            ++exp;
            b=b / 10.0;
        }
    }
    else if ( b == 0.0 )
    {
        exp=0;
    }
    else if ( b < 1.0 )
    {
        while(b < 1.0)
        {
            --exp;
            b=b*10.0;
        }
    }

    

    switch (cnvtype)
    {
        case 'E':
        case 'e':
            formato_STDIO = 0;
            break;
        case 'f':
        case 'F':
            if ( exp >= 0 ) formato_STDIO = 1; else formato_STDIO = -1;
            break;
        default:
           
            if ( exp >= 0 )
            {
                if ( nprecisao > exp )
                {
                    formato_STDIO=1;
                }
                else
                {
                    formato_STDIO=0;
                }
            }
            else
            {
                /*  if ( nprecisao > (-(exp+1) ) ) { */
                if ( exp >= -4)
                {
                    formato_STDIO=-1;
                }
                else
                {
                    formato_STDIO=0;
                }
            }
            break;
    }
   
    switch (formato_STDIO)
    {
        case 0:   
            if (nprecisao < DBL_MANT_DIG)
            {
                j = nprecisao;
            }
            else
            {
                j=DBL_MANT_DIG;
            }
            round = 1.0/2.0;
            i = 0;
            while (++i <= j)
            {
                round = round/10.0;
            }
            b = b + round;
            if (b >= 10.0)
            {
                b = b/10.0;
                exp = exp + 1;
            }
            break;

            case 1:      
                        
                if (exp + nprecisao < DBL_MANT_DIG) 
                {
                    j = exp + nprecisao;
                }
                else
                {
                    j = DBL_MANT_DIG;
                }
                round = 0.5;
                i = 0;
                while (i++ < j)
                {
                    round = round/10;
                }
                b = b + round;
                if (b >= 10.0)
                {
                    b = b/10.0;
                    exp = exp + 1;
                }
                break;

        case -1:   
            if (nprecisao < DBL_MANT_DIG)
            {
                j = nprecisao + exp + 1;
            }
            else
            {
                j = DBL_MANT_DIG;
            }
            round = 5.0;
            i = 0;
            while (i++ < j)
            {
                round = round/10;
            }
            if (j >= 0)
            {
                b = b + round;
            }
            if (b >= 10.0)
            {
                b = b/10.0;
                exp = exp + 1;
            }
            if (exp >= 0)
            {
                formato_STDIO = 1;
            }
            break;
    }
   

    if (formato_STDIO==-1)
    {
        
        resultado[0]=sinalSTDIO;
        resultado[1]='0';
        resultado[2]='.';
        resultado[3]=0x00;
        while (++exp)
        {
            --nprecisao;
            strcat(resultado,"0");
        }
        i=b;
        --nprecisao;
        trabalho[0] = (char)('0' + i % 10);
        trabalho[1] = 0x00;
        strcat(resultado,trabalho);

        pdigitos = nprecisao;

        while (pdigitos-- > 0)
        {
            b = b - i;
            b = b * 10.0;
            i = b;
            trabalho[0] = (char)('0' + i % 10);
            trabalho[1] = 0x00;
            strcat(resultado,trabalho);
        }
    }
    
    else if (formato_STDIO==+1)
    {
        i = b;
        resultado[0] = sinalSTDIO;
        resultado[1] = '\0';
        trabalho[0] = (char)('0' + i % 10);
        trabalho[1] = 0x00;
        strcat(resultado,trabalho);
        nprecisao = nprecisao + exp;
        pdigitos = nprecisao ;

        while (pdigitos-- > 0)
        {
            if ( ((nprecisao-pdigitos-1)==exp)  )
            {
                strcat(resultado,".");
            }
            b = b - i;
            b = b * 10.0;
            
            if ( ( (nprecisao-pdigitos-1) > exp) && (b < 0.1E-15 ) )
            {
               if (cnvtype != 'G' && cnvtype != 'g')
               {
                   strcat(resultado,"0");
               }
            }
            else
            {
                i = b;
                trabalho[0] = (char)('0' + i % 10);
                trabalho[1] = 0x00;
                strcat(resultado,trabalho);
            }
        }
    }
   
    else
    {
        i = b;
        resultado[0] = sinalSTDIO;
        resultado[1] = '\0';
        trabalho[0] = (char)('0' + i % 10);
        trabalho[1] = 0x00;
        strcat(resultado,trabalho);
        strcat(resultado,".");

        pdigitos = nprecisao;

        while (pdigitos-- > 0)
        {
            b = b - i;
            b = b * 10.0;
            i = b;
            trabalho[0] = (char)('0' + i % 10);
            trabalho[1] = 0x00;
            strcat(resultado,trabalho);
        }
    }

    if (formato_STDIO==0)
    { 
        trabalho[0] = 'E';
        if ( exp < 0 )
        {
            exp = -exp;
            trabalho[1]= '-';
        }
        else
        {
            trabalho[1]= '+';
        }
        trabalho[2] = (char)('0' + (exp/10) % 10);
        trabalho[3] = (char)('0' + exp % 10);
        trabalho[4] = 0x00;
        strcat(resultado, trabalho);
    }
   
    if(resultado[0] == ' ')strcpy(trabalho,resultado+1); else strcpy(trabalho,resultado);
    pdigitos=nlargura-tamanhostring(trabalho);
    resultado[0]= 0x00;
    while(pdigitos>0)
    {
        strcat(resultado," ");
        pdigitos--;
    }
    strcat(resultado,trabalho);
    return;
}
