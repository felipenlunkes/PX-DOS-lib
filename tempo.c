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
/*********************************************************************/

#include "tempo.h"
#include "stdarg.h"
#include "stdio.h"
#include "stddef.h"

/* O PX-DOS e o MS-DOS usam a mesma interface de tempo, em algumas circunstâncias */

#ifdef __PXDOS__
#define __MSDOS__
#endif

#ifdef __MSDOS__

#define CTYP

void CTYP __datahora(void *ptr);
#endif



static int isleap(unsigned an)
{
   return an % 400 == 0 || (an % 4 == 0 && an % 100 != 0);
}

static unsigned meses_para_dias(unsigned mes)
{
   return (mes * 3057 - 3007) / 100;
}

static long anos_para_dias (unsigned an)
{
   return an * 365L + an / 4 - an / 100 + an / 400;
}

static long AnoMesDia_para_escalar(unsigned an, unsigned ms, unsigned dia)
{
   long escalar;

   escalar = dia + meses_para_dias(ms);
   if ( ms > 2 )                        
      escalar -= isleap(an) ? 1 : 2;
   an--;
   escalar += anos_para_dias(an);
   return (escalar);
}

static void escalar_para_ano_mes_dia(long escalar,
                          unsigned *pano,
                          unsigned *pmes_tempo,
                          unsigned *pdia)
{
   unsigned n;               

   n = (unsigned)((escalar * 400L) / 146097L);
   while (anos_para_dias(n) < escalar)
   {
      n++;
   }
   for ( n = (unsigned)((escalar * 400L) / 146097L); anos_para_dias(n) < escalar; )
      n++;                          /* 146097 == anos_para_dias(400) */
   *pano = n;
   n = (unsigned)(escalar - anos_para_dias(n-1));
   if ( n > 59 ) {                       
      n += 2;
      if ( isleap(*pano) )
         n -= n > 62 ? 1 : 2;
   }
   *pmes_tempo = (n * 100 + 3007) / 3057;  /* inverso de meses_para_dias() */
   *pdia = n - meses_para_dias(*pmes_tempo);
   return;
}

tempo_t hora(tempo_t *timer)
{
    tempo_t tt;

#if defined(__MSDOS__)
    struct {
        int ano;
        int mes;
        int dia;
        int horas;
        int minutosutos;
        int segundos;
        int decimos_de_segundos;
    } dt;
#endif
#if defined(__MVS__) || defined(__CMS__)
    unsigned int rlg[2];
#endif

#ifdef __MSDOS__
    __datahora(&dt);
#endif
#if defined(__MVS__) || defined(__CMS__)
    tt = __getclk(rlg);
#elif !defined(__WIN32__)

    {
        tt = AnoMesDia_para_escalar(dt.ano, dt.mes, dt.dia)
             - AnoMesDia_para_escalar(1970, 1, 1);
        tt = tt * 24 + dt.horas;
        tt = tt * 60 + dt.minutosutos;
        tt = tt * 60 + dt.segundos;
    }
#endif
    if (timer != NULL)
    {
        *timer = tt;
    }
    return (tt);
}

relogio_t relogio(void)
{
    return ((relogio_t)-1);
}

double diferencaTempo(tempo_t tempo1, tempo_t tempo0)
{
    return ((double)(tempo1 - tempo0));
}

tempo_t fazertempo(struct tm *ponteiro_tempo)
{
    tempo_t tt;

    if ((ponteiro_tempo->tm_ano < 70) || (ponteiro_tempo->tm_ano > 120))
    {
        tt = (tempo_t)-1;
    }
    else
    {
        tt = AnoMesDia_para_escalar(ponteiro_tempo->tm_ano + 1900,
                           ponteiro_tempo->tm_mes + 1,
                           ponteiro_tempo->tm_mdia)
             - AnoMesDia_para_escalar(1970, 1, 1);
        tt = tt * 24 + ponteiro_tempo->tm_hora;
        tt = tt * 60 + ponteiro_tempo->tm_minutos;
        tt = tt * 60 + ponteiro_tempo->tm_sec;
    }
    return (tt);
}

char *asctempo(const struct tm *ponteiro_tempo)
{
    static const char wdia_nome[7][3] = {
          "Dom", "Seg", "Ter", "Qua", "Qui", "Sex", "Sab"
    };
    static const char mes_name[12][3] = {
          "Jan", "Fev", "Mar", "Abr", "Mai", "Jun",
          "Jul", "Ago", "Set", "Out", "Nov", "Dez"
    };
    static char resultado[26];

    sprintf(resultado, "%.3s %.3s%3d %.2d:%.2d:%.2d %d\n",
          wdia_nome[ponteiro_tempo->tm_wdia],
          mes_name[ponteiro_tempo->tm_mes],
          ponteiro_tempo->tm_mdia, ponteiro_tempo->tm_hora,
          ponteiro_tempo->tm_minutos, ponteiro_tempo->tm_sec,
          1900 + ponteiro_tempo->tm_ano);
    return resultado;
}

char *horac(const tempo_t *timer)
{
    return (asctempo(horalocal(timer)));
}

struct tm *horagmt(const tempo_t *timer)
{
    return (horalocal(timer));
}


#define dow(y,m,d) \
  ((((((m)+9)%12+1)<<4)%27 + (d) + 1 + \
  ((y)%400+400) + ((y)%400+400)/4 - ((y)%400+400)/100 + \
  (((m)<=2) ? ( \
  (((((y)%4)==0) && (((y)%100)!=0)) || (((y)%400)==0)) \
  ? 5 : 6) : 0)) % 7)

static struct tm tms;

struct tm *horalocal(const tempo_t *timer)
{
    unsigned an, ms, da;
    unsigned long segundos;
    unsigned long dias;

    dias = *timer / (60L*60*24);
    segundos = *timer % (60L*60*24);
    escalar_para_ano_mes_dia(dias + AnoMesDia_para_escalar(1970, 1, 1), &an, &ms, &da);
    tms.tm_ano = an - 1900;
    tms.tm_mes = ms - 1;
    tms.tm_mdia = da;
    tms.tm_ydia = (int)(AnoMesDia_para_escalar(tms.tm_ano + 1900, ms, da)
                  - AnoMesDia_para_escalar(tms.tm_ano + 1900, 1, 1));
    tms.tm_wdia = dow(tms.tm_ano + 1900, ms, da);
    tms.tm_eHorarioDeVerao = -1;
    tms.tm_sec = (int)(segundos % 60);
    segundos /= 60;
    tms.tm_minutos = (int)(segundos % 60);
    segundos /= 60;
    tms.tm_hora = (int)segundos;
    return (&tms);
}



static char *adia[] = {
    "Dom", "Seg", "Ter", "Qua", "Qui", "Sex", "Sab"
};

static char *dia[] = {
    "Domingo", "Segunda", "Terca", "Quarta",
    "Quinta", "Sexta", "Sabado"
};

static char *amesth[] = {
    "Jan", "Fev", "Mar", "Abr", "Mai", "Jun",
    "Jul", "Ago", "Set", "Out", "Nov", "Dez"
};

static char *mes[] = {
    "Janeiro", "Fevereiro", "Marco", "Abril", "Maio", "Junho",
    "Julho", "Agosto", "Setembro", "Outubro", "Novembro", "Dezembro"
};

static char *__tznome[2] = { "" "" };
static char buf[26];

static void strfmt(char *str, const char *fmt, ...);


tamanho_t strfTempo(char *s, tamanho_t maxs, const char *f, const struct tm *t)
{
      int w;
      char *p, *q, *r;

      p = s;
      q = s + maxs - 1;
      while ((*f != '\0'))
      {
            if (*f++ == '%')
            {
                  r = buf;
                  switch (*f++)
                  {
                  case '%' :
                        r = "%";
                        break;

                  case 'a' :
                        r = adia[t->tm_wdia];
                        break;

                  case 'A' :
                        r = dia[t->tm_wdia];
                        break;

                  case 'b' :
                        r = amesth[t->tm_mes];
                        break;

                  case 'B' :
                        r = mes[t->tm_mes];
                        break;

                  case 'c' :
                        strfmt(r, "%0 %0 %2 %2:%2:%2 %4",
                              adia[t->tm_wdia], amesth[t->tm_mes],
                              t->tm_mdia,t->tm_hora, t->tm_minutos,
                              t->tm_sec, t->tm_ano+1900);
                        break;

                  case 'd' :
                        strfmt(r,"%2",t->tm_mdia);
                        break;

                  case 'H' :
                        strfmt(r,"%2",t->tm_hora);
                        break;

                  case 'I' :
                        strfmt(r,"%2",(t->tm_hora%12)?t->tm_hora%12:12);
                        break;

                  case 'j' :
                        strfmt(r,"%3",t->tm_ydia+1);
                        break;

                  case 'm' :
                        strfmt(r,"%2",t->tm_mes+1);
                        break;

                  case 'M' :
                        strfmt(r,"%2",t->tm_minutos);
                        break;

                  case 'p' :
                        r = (t->tm_hora>11)?"PM":"AM";
                        break;

                  case 'S' :
                        strfmt(r,"%2",t->tm_sec);
                        break;

                  case 'U' :
                        w = t->tm_ydia/7;
                        if (t->tm_ydia%7 > t->tm_wdia)
                              w++;
                        strfmt(r, "%2", w);
                        break;

                  case 'W' :
                        w = t->tm_ydia/7;
                        if (t->tm_ydia%7 > (t->tm_wdia+6)%7)
                              w++;
                        strfmt(r, "%2", w);
                        break;

                  case 'w' :
                        strfmt(r,"%1",t->tm_wdia);
                        break;

                  case 'x' :
                        strfmt(r, "%3s %3s %2 %4", adia[t->tm_wdia],
                              amesth[t->tm_mes], t->tm_mdia, t->tm_ano+1900);
                        break;

                  case 'X' :
                        strfmt(r, "%2:%2:%2", t->tm_hora,
                              t->tm_minutos, t->tm_sec);
                        break;

                  case 'y' :
                        strfmt(r,"%2",t->tm_ano%100);
                        break;

                  case 'Y' :
                        strfmt(r,"%4",t->tm_ano+1900);
                        break;

                  case 'Z' :
                        r = (t->tm_eHorarioDeVerao) ? __tznome[1] : __tznome[0];
                        break;

                  default:
                        buf[0] = '%';     /* Reconstruir formato_STDIO */
                        buf[1] = f[-1];
                        buf[2] = '\0';
                        if (buf[1] == 0)
                              f--;      
                  }
                  while (*r)
                  {
                        if (p == q)
                        {
                              *q = '\0';
                              return 0;
                        }
                        *p++ = *r++;
                  }
            }
            else
            {
                  if (p == q)
                  {
                        *q = '\0';
                        return 0;
                  }
                  *p++ = f[-1];
            }
      }
      *p = '\0';
      return (tamanho_t)(p - s);
}

static int pow[5] = { 1, 10, 100, 1000, 10000 };


static void strfmt(char *str, const char *fmt, ...)
{
      int ival, itamanho;
      char *sval;
      va_list vp;

      va_start(vp, fmt);
      while (*fmt)
      {
            if (*fmt++ == '%')
            {
                  itamanho = *fmt++ - '0';
                  if (itamanho == 0)                /* zero significa arg de string */
                  {
                        sval = va_arg(vp, char*);
                        while (*sval)
                              *str++ = *sval++;
                  }
                  else                          
                  {
                        ival = va_arg(vp, int);
                        while (itamanho)
                        {
                              ival %= pow[itamanho--];
                              *str++ = (char)('0' + ival / pow[itamanho]);
                        }
                  }
            }
            else  *str++ = fmt[-1];
      }
      *str = '\0';
      va_end(vp);
}

