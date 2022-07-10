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

#ifndef __STDIO_INCLUIDO
#define __STDIO_INCLUIDO

#ifndef __TAMANHO_T_DEFINIDO
#define __TAMANHO_T_DEFINIDO

typedef unsigned int tamanho_t;

#endif


typedef struct
{

#if (defined(__MSDOS__) || defined(__DOS__) || defined(__POWERC))
    int ManipuladorArquivo; /* Manipulador de Arquivos estilo DOS */

#endif

#if (defined(__MVS__) || defined(__CMS__))
    void *ManipuladorArquivo;
    int recfm;
    int estilo;
    int lrecl;
    char nome_dd[9];
    char pxdosMem[9];
#endif
    int BinRapido;  /* 1 = fazer DosRead */
    int TextoRapido;
    int modoTexto; 
    int Numero_Arquivo_Interno; 
    unsigned long InicioBufferR; 
    char *fbuf;     
    tamanho_t tamanhoabuf;  
    char *ProximoLista;    
    char *fimbuffer;   
    int ErronoArquivo;   
    int eofInd;    
    int ungetCh; 
    int TecnicaBuffer;    
    char *intBuffer; 
    int noNl;     
    int modo;     
    int atualizar;   
    int EsteBuffer; 
    int arquivoPermanente;  
    int Arquivo_Aberto; 
    char StrMODO[4];
} ARQUIVO;

typedef unsigned long fpos_t;

#define NULL ((void *)0)
#define ARQUIVOS_MAXIMOS 260
#define ARQUIVOS_ABERTOS_MAXIMOS 256
#define _IOFBF 1
#define _IOLBF 2
#define _IONBF 3

#if defined(__MVS__) || defined(__CMS__)

#define TamanhoBuffer_DEFINE 32768
#else
#define TamanhoBuffer_DEFINE 6144
#endif

#define EOF -1
#define L_tmpnam ARQUIVOS_MAXIMOS
#define TMP_MAX 25
#define DEFINIR_OBTER 0
#define OBTER_COR 1
#define OBTER_FIM 2
#define __N_ARQUIVO (ARQUIVOS_ABERTOS_MAXIMOS - 3)
#define __MODO_ESCRITA 1
#define __MODO_LEITURA 2

extern ARQUIVO *stdin;
extern ARQUIVO *stdout;
extern ARQUIVO *stderr;

extern ARQUIVO *__Arquivos_Usuario[__N_ARQUIVO];

int printf(const char *formato_STDIO, ...);
ARQUIVO *abrir(const char *nome_de_arquivo, const char *modo);
int fechar(ARQUIVO *stream);
tamanho_t ArquivoLer(void *ptr, tamanho_t tamanho_STDIO, tamanho_t nmemb, ARQUIVO *stream);
tamanho_t escArquivo(const void *ptr, tamanho_t tamanho_STDIO, tamanho_t nmemb, ARQUIVO *stream);
int fputc(int c, ARQUIVO *stream);
int fputs(const char *s, ARQUIVO *stream);
int fprintf(ARQUIVO *stream, const char *formato_STDIO, ...);
int vfprintf(ARQUIVO *stream, const char *formato_STDIO, char *arg);
int remover(const char *nome_de_arquivo);
int renomear(const char *antigo, const char *new);
int sprintf(char *s, const char *formato_STDIO, ...);
int vsprintf(char *s, const char *formato_STDIO, char *arg);
char *fgets(char *s, int n, ARQUIVO *stream);
int ungetc(int c, ARQUIVO *stream);
int fgetc(ARQUIVO *stream);
int fobter(ARQUIVO *stream, long int offset, int deOnde);
long int ftell(ARQUIVO *stream);
int defposicaoarquivo(ARQUIVO *stream, const fpos_t *pos);
int obtposicaoarquivo(ARQUIVO *stream, fpos_t *pos);
void rewind(ARQUIVO *stream);
void limparErro(ARQUIVO *stream);
void manipularErro(const char *s);
int setvbuf(ARQUIVO *stream, char *buf, int modo, tamanho_t tamanho_STDIO);
int definirBuffer(ARQUIVO *stream, char *buf);
ARQUIVO *freopen(const char *nome_de_arquivo, const char *modo, ARQUIVO *stream);
int fflush(ARQUIVO *stream);
char *NomeTemporarioDOS(char *s);
ARQUIVO *arquivoTemporarioDOS(void);
int fscanf(ARQUIVO *stream, const char *formato_STDIO, ...);
int scanf(const char *formato_STDIO, ...);
int sscanf(const char *s, const char *formato_STDIO, ...);
char *gets(char *s);
int puts(const char *s);

#ifndef __POWERC
int getchar(void);
int putchar(int c);
int getc(ARQUIVO *stream);
int putc(int c, ARQUIVO *stream);
int feof(ARQUIVO *stream);
int ferror(ARQUIVO *stream);
#endif

#define getchar() (getc(stdin))
#define putchar(c) (putc((c), stdout))
#define getc(stream) (fgetc((stream)))
#define putc(c, stream) (fputc((c), (stream)))
#define feof(stream) ((stream)->eofInd)
#define ferror(stream) ((stream)->ErronoArquivo)

#endif


