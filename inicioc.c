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
#include "stdlib.h"
#include "string.h"



#define MAXPARMS 19 /* Número máximo de argumentos */


#define CTYP




extern unsigned char *__ponteirodeambiente;
extern unsigned short __verso;


int main(int argc, char **argv);

void __sair(int status);
void CTYP __saira(int status);

#if !defined(__MVS__) && !defined(__CMS__)
static char buffer1[TamanhoBuffer_DEFINE + 8];
static char buffer2[TamanhoBuffer_DEFINE + 8];
static char buffer3[TamanhoBuffer_DEFINE + 8];
#endif


#if defined(__CMS__)
int __iniciar(char *p, char *nomePrograma)
#elif defined(__MVS__)
int __iniciar(char *p, char *nomePrograma, int tso)
#elif defined(__PXDOS__)
int __iniciar(int *i1, int *i2, int *i3, POS_EPARMS *exep)
#else
int CTYP __iniciar(char *p)
#endif
{
#ifdef __PXDOS__
    char *p;
#endif
    int x;
    int argc;
    static char *argv[MAXPARMS + 1];
    int rc;

#ifdef __MSDOS__
    unsigned char *env;
#endif


#ifdef __PXDOS__
    p = exep->psp;
    __abscor = exep->abscor;
    __vidptr = ABSADDR(0xb8000);
#endif


#if !defined(__MVS__) && !defined(__CMS__)

    stdin->ManipuladorArquivo = 0;
    stdout->ManipuladorArquivo = 1;
    stderr->ManipuladorArquivo = 2;

    stdin->BinRapido = 0;
    stdin->TextoRapido = 0;
    stdin->modoTexto = 1;
    stdin->Numero_Arquivo_Interno = 0;
    stdin->InicioBufferR = 0;
    stdin->TecnicaBuffer = _IOLBF;
    stdin->intBuffer = buffer1;
    stdin->fbuf = stdin->intBuffer + 2;
    *stdin->fbuf++ = '\0';
    *stdin->fbuf++ = '\0';
    stdin->tamanhoabuf = TamanhoBuffer_DEFINE;
    stdin->fimbuffer = stdin->fbuf + stdin->tamanhoabuf;
    *stdin->fimbuffer = '\n';
    stdin->noNl = 0;
    stdin->ProximoLista = stdin->fimbuffer;
    stdin->InicioBufferR = -stdin->tamanhoabuf;
    stdin->modo = __MODO_LEITURA;
    stdin->ungetCh = -1;
    stdin->atualizar = 0;
    stdin->EsteBuffer = 0;
    stdin->arquivoPermanente = 1;
    stdin->Arquivo_Aberto = 1;

    stdout->BinRapido = 0;
    stdout->TextoRapido = 0;
    stdout->modoTexto = 1;
    stdout->TecnicaBuffer = _IOLBF;
    stdout->intBuffer = buffer2;
    stdout->fbuf = stdout->intBuffer;
    *stdout->fbuf++ = '\0';
    *stdout->fbuf++ = '\0';
    stdout->tamanhoabuf = TamanhoBuffer_DEFINE;
    stdout->fimbuffer = stdout->fbuf + stdout->tamanhoabuf;
    *stdout->fimbuffer = '\n';
    stdout->noNl = 0;
    stdout->ProximoLista = stdout->fbuf;
    stdout->InicioBufferR = 0;
    stdout->modo = __MODO_ESCRITA;
    stdout->atualizar = 0;
    stdout->EsteBuffer = 0;
    stdout->arquivoPermanente = 1;
    stdout->Arquivo_Aberto = 1;

    stderr->BinRapido = 0;
    stderr->TextoRapido = 0;
    stderr->modoTexto = 1;
    stderr->TecnicaBuffer = _IOLBF;
    stderr->intBuffer = buffer3;
    stderr->fbuf = stderr->intBuffer;
    *stderr->fbuf++ = '\0';
    *stderr->fbuf++ = '\0';
    stderr->tamanhoabuf = TamanhoBuffer_DEFINE;
    stderr->fimbuffer = stderr->fbuf + stderr->tamanhoabuf;
    *stderr->fimbuffer = '\n';
    stderr->noNl = 0;
    stderr->ProximoLista = stderr->fbuf;
    stderr->InicioBufferR = 0;
    stderr->modo = __MODO_ESCRITA;
    stderr->atualizar = 0;
    stderr->EsteBuffer = 0;
    stderr->arquivoPermanente = 1;
    stderr->Arquivo_Aberto = 1;

	#endif

    for (x=0; x < __N_ARQUIVO; x++)
    {
        __Arquivos_Usuario[x] = NULL;
    }

#if defined(__MSDOS__) || defined(__PXDOS__)
    argv[0] = "";

#ifdef __MSDOS__
    if(__verso > 0x300)
    {
        env=__ponteirodeambiente;
        while (1)
        {
            if (*env++ == '\0' && *env++ == '\0')
            {
                if (*(unsigned short *)env != 0)
                {
                    argv[0] = (char *)env + 2;
                }
                break;
            }
        }
    }
#endif
    p = p + 0x80;
    p[*p + 1] = '\0';
    p++;
#endif
    while (*p == ' ')
    {
        p++;
    }
    if (*p == '\0')
    {
        argv[1] = NULL;
        argc = 1;
    }
    else
    {
        for (x = 1; x < MAXPARMS; )
        {
            char srch = ' ';

            if (*p == '"')
            {
                p++;
                srch = '"';
            }
            argv[x] = p;
            x++;
            p = strchr(p, srch);
            if (p == NULL)
            {
                break;
            }
            else
            {
                *p = '\0';
                p++;
                while (*p == ' ') p++;
                if (*p == '\0') break; 
            }
        }
        argv[x] = NULL;
        argc = x;
    }
#ifdef PXDOS_ENTRADA_PRINCIPAL
    *i1 = argc;
    *i2 = (int)argv;
    return (0);
#else
    rc = main(argc, argv);
    __sair(rc);
    return (rc);
#endif
}

void __sair(int status)
{
    int x;

#if 0
    for (x = __NATSAIR - 1; x >= 0; x--)
    {
        if (__sairUsuario[x] != 0)
        {
            (__sairUsuario[x])();
        }
    }
#endif
    for (x = 0; x < __N_ARQUIVO; x++)
    {
        if (__Arquivos_Usuario[x] != NULL)
        {
            fechar(__Arquivos_Usuario[x]);
        }
    }
    fflush(stdout);
    fflush(stderr);



    __saira(status);

    return;
}
