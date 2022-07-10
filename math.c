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

#include "math.h"
#include "float.h"
#include "errno.h"


static const double pi   = 3.1415926535897932384626433832795;
static const double ln10 = 2.3025850929940456840179914546844;
static const double ln2 = 0.69314718055994530941723212145818 ;


double ceil(double x)
{
    int y;

    y = (int)x;
    if ((double)y < x)
    {
        y++;
    }
    return ((double)y);
}

#ifdef fabs
#undef fabs
#endif
double fabs(double x)
{
    if (x < 0.0)
    {
        x = -x;
    }
    return (x);
}

double floor(double x)
{
    int y;

    if (x < 0.0)
    {
        y = (int)x;
        if ((double)y != x)
        {
            y--;
        }
    }
    else
    {
        y = (int)x;
    }
    return ((double)y);
}

double fmod(double x, double y)
{
    int imod;
    if(y == 0.0) return (0.0);
    imod = x / y;
    return ((double)x-((double)imod*y));
}

#ifdef acos
#undef acos
#endif

double acos(double x)
{

/*
*/

    if ( fabs(x) > 1.0 ) 
    {
        errno=EDOM;
        return (HUGE_VAL);
    }
    if ( x < 0.0 ) return ( pi - acos(-x) ) ;

    return ( asin ( sqrt(1.0 - x*x) ) );

}

#ifdef asin
#undef asin
#endif


double asin (double y)
{
    int i;
    double term,resposta,trabalho,x,powx,coef;

    x = y;


    if (x <0.0 ) return ( -asin(-x) );

    if ( x > 1.0 )
    {
        errno=EDOM;
        return(HUGE_VAL);
    }


    if( x > 0.75 )
    {
        x = ( sqrt(1.0 - (x*x) ) );
        return((pi/2.0)-asin(x));
    }


    i=1;
    resposta=x;
    term = 1;
    coef = 1;
    powx = x;

    while (1)
    {
        trabalho = i;
        coef = (coef * trabalho)/(trabalho+1);
        powx = powx * x * x;
        term =  coef * powx / (trabalho + 2.0);
        if ( resposta == (resposta + term) )break;
        resposta = resposta + (term);
        i+=2;
    }

    return(resposta);
}


#ifdef atan
#undef atan
#endif


double atan (double x)
{
    int i;
    double term,resposta,trabalho,powx;


    if ( x<0.0 ) return ( -atan(-x) );



    if ( x > 1.0 ) return ((pi/2) - atan(1.0/x));



    if( x > (2.0 - sqrt(3.0)))
    return( (pi/6.0) + atan( ( x * sqrt(3.0)-1.0 ) / (x + sqrt(3.0) ) ) );


    i=1;
    resposta=x;
    term = x;
    powx = x;

    while (1)
    {
        trabalho = i;
        powx = 0.0 - powx * x * x;
        term = powx / (trabalho + 2.0);
        if ( resposta == (resposta + term) )break;
        resposta = resposta + (term);
        i+=2;
    }

    return(resposta);

}



double atan2(double y,double x)
{
    return (x >= y) ?
               (x >= -y ? atan(y/x) : -pi/2 - atan(x/y))
              :
               (x >= -y ? pi/2 - atan(x/y)
                        : (y >= 0) ? pi + atan(y/x)
                                   : -pi + atan(y/x));
}


#ifdef cos
#undef cos
#endif
double cos(double x)
{


    int i;
    double term,resposta,trabalho,x1;



    i = x/(2*pi);
    x1 =  x - (i * (2.0 * pi));

    i=1;
    term=resposta=1;


    while (1)
    {
        trabalho = i;
        term = -(term * x1 * x1)/(trabalho * (trabalho + 1.0));
        if ( resposta == (resposta + term) )break;
        resposta = resposta + term;
        i += 2;
    }

    return(resposta);

}

#ifdef sin
#undef sin
#endif
double sin(double x)
{


    int i;
    double term,resposta,trabalho,x1;


    i = x/(2.0*pi);
    x1 =  x - (i * (2.0 * pi));

    i=1;
    term=resposta=x1;

    while (1)
    {
        trabalho = i+1;
        term = -(term * x1 * x1)/(trabalho * (trabalho + 1.0));
        if ( resposta == (resposta + term) )break;
        resposta = resposta + term;
        i = i+2;
    }

    return(resposta);
}

#ifdef tan
#undef tan
#endif
double tan (double x)
{

    double temp;

    temp=cos(x);
    if (temp == 0.0 )
    {
        
        return (HUGE_VAL);
    }
    return ( sin(x)/cos(x) );
}


double cosh(double x)
{
    double dexpx;

    dexpx = exp(x);

    return( 0.5 * (dexpx + (1.0/dexpx) ) );

}

double sinh(double x)
{
    double dexpx;

    dexpx = exp(x);

    return( 0.5 * (dexpx - (1.0/dexpx) ) );
}



double tanh(double x)
{
    double dexp2;

    dexp2 = exp( -2.0 * x);
    return ( (1.0  - dexp2) /  (1.0 + dexp2) );
}


double exp (double x)
{
    int i;
    double term,resposta,trabalho;

    i=2;
    term=x;
    resposta=x;

    while (1)
    {
        trabalho = i;
        term =  (term * x)/trabalho;
        if ( resposta == (resposta + term) )break;
        resposta = resposta + (term);
        i++;
    }

    resposta=resposta+1.0;
    return(resposta);
}


double log (double x)
{
    int i,scale;
    double term,resposta,trabalho,xs;

    if (x <= 0 )
    {
       
        errno=EDOM;
        return (HUGE_VAL);
    }
    if( x == 1.0)return(0.0);


    xs = frexp(x,&scale);
    xs = (1.0 * xs) - 1.0;
    scale = scale - 0;

    i=2;
    term=resposta=xs;

    while (1)
    {
        trabalho = i;
        term = - (term * xs);
        if ( resposta == (resposta + (term/trabalho)) )break;
        resposta = resposta + (term/trabalho);
        i++;
    }

    resposta = resposta + (double)scale * ln2;
    return(resposta);
}


double log10(double x)
{
    return ( log(x) / ln10 );
}




double pow(double x,double y)
{
    int j,neg;
    double yy,xx;
    neg=0;
    j=y;
    yy=j;
    if( yy == y) {
        xx = x;
        if ( y < 0 ){neg = 1; j = -j;}
        if ( y == 0) return (1.0);
        --j;
        while(j>0){ xx=xx * x; j--;}
        if(neg)xx=1.0/xx;
        return (xx);
    }
    if (x < 0.0)
    {
         errno=EDOM;
         return(0.0);
    }
    if (y == 0.0) return (1.0);

    return (exp(y*log(x)));
}

#ifdef sqrt
#undef sqrt
#endif


double sqrt(double x)
{
    double xs,yn,ynn;
    double pow1;
    int i;

    if (x < 0.0)
    {
        errno=EDOM;
        return(0.0);
    }
    if (x == 0.0) return (0.0);



    xs=x;
    pow1=1;

    while(xs<1.0){xs=xs*4.0;pow1=pow1/2.0;}
    while(xs>=4.0){xs=xs/4.0;pow1=pow1*2.0;}


    i=0;
    yn = xs/2.0;
    ynn = 0;
    while(1)
    {
        ynn = (yn + xs/yn)*0.5;
        if ( fabs(ynn-yn) <= 10.0 * DBL_MIN ) break; else yn=ynn;
        if ( i > 10  ) break; else i++ ;
    }
    return (ynn*pow1);
}


double frexp(double x, int *exp)
{

    union dblhex
    {
        double d;
        unsigned short s[4];
    };
    union dblhex split;

    if ( x == 0.0)
    {
        exp=0;
        return (0.0);
    }

    split.d = x;
    *exp = (((split.s[0] >> 8) & 0x007f)-64) * 4;
    split.s[0] = split.s[0] & 0x80ff;
    split.s[0] = split.s[0] | 0x4000;
   
    while ( (fabs(split.d) < 0.5) && (split.d != 0) )
    {
        split.d = split.d * 2;
        *exp =( *exp ) - 1;
    }
    /*    */
    return(split.d);
}
double ldexp(double x, int exp)
{

    int bin_exp,hex_exp,adj_exp;
    union dblhex
    {
        double d;
        unsigned short s[4];
    };
    union dblhex split;

    split.d = frexp(x,&bin_exp);
    bin_exp = bin_exp + exp; 

    hex_exp =  (bin_exp / 4);
    adj_exp =  bin_exp - (hex_exp * 4);
    if (adj_exp < 0){ hex_exp=hex_exp -1; adj_exp = 4 + adj_exp;}
    split.s[0] = split.s[0] & 0x80ff;
    split.s[0] = split.s[0] | (((hex_exp+64)  << 8) & 0x7f00);
   
    while ( adj_exp > 0 )
    {
        split.d = split.d * 2;
        --adj_exp;
    }
    /**/
    return(split.d);
}

double modf(double valor, double *iptr)
{
    int neg = 0;
    long i;

    if (valor < 0)
    {
        neg = 1;
        valor = -valor;
    }
    i = (long)valor;
    valor -= i;
    if (neg)
    {
        valor = -valor;
        i = -i;
    }
    *iptr = i;
    return (valor);
}
