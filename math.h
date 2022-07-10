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

#ifndef __MATH_INCLUIDO
#define __MATH_INCLUIDO

#if defined(__MVS__) || defined (__CMS__)

#define HUGE_VAL 9.999999999999999999999E72

#else

#define HUGE_VAL 9.999999999999999999999E72

#endif

double ceil(double x);
double fabs(double x);
double floor(double x);
double fmod(double x, double y);




double atan2(double y, double x);
double frexp(double valor, int *exp);
double ldexp(double x, int exp);
double modf(double valor, double *iptr);


double pow(double x, double y);
double sqrt(double x);
double acos(double x);
double asin(double x);
double atan(double x);
double cos(double x);
double sin(double x);
double tan(double x);
double cosh(double x);
double sinh(double x);
double tanh(double x);
double exp(double x);
double log(double x);
double log10(double x);



#ifdef __BORLANDC__
double __sin__(double x);
#define sin(x) (__sin__((x)))
double __cos__(double x);
#define cos(x) (__cos__((x)))
#endif

#endif

