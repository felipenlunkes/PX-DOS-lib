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



#ifdef __MSDOS__

#ifdef __WATCOMC__
#define CTYP __cdecl
#else
#define CTYP
#endif

unsigned long CTYP __divide(unsigned long x, unsigned long y)
{
    unsigned int a;
    unsigned int b;
    unsigned int c;
    unsigned int d;
    unsigned int e;
    unsigned int f;
    unsigned int g = 0;
    unsigned int h;

    b = (unsigned int)(y & 0xffffU);
    a = (unsigned int)(y >> 16);
    c = -b;
    d = a;
    if (b > 0)
    {
        d++;
    }
    f = 1;
    while (f != 0)
    {
        e = (unsigned int)(x >> 16);
        h = (unsigned int)(x & 0xffffU);
        f = e / d;
        g += f;
        e = e % d;
        x = ((unsigned long)e << 16) + (unsigned long)c * f + h;
    }
    if (x >= y)
    {
        g++;
    }
    return ((unsigned long)g);
}

unsigned long CTYP __modulo(unsigned long x, unsigned long y)
{
    unsigned int a;
    unsigned int b;
    unsigned int c;
    unsigned int d;
    unsigned int e;
    unsigned int f;
    unsigned int g = 0;
    unsigned int h;

    b = (unsigned int)(y & 0xffffU);
    a = (unsigned int)(y >> 16);
    c = -b;
    d = a;
    if (b > 0)
    {
        d++;
    }
    f = 1;
    while (f != 0)
    {
        e = (unsigned int)(x >> 16);
        h = (unsigned int)(x & 0xffffU);
        f = e / d;
        g += f;
        e = e % d;
        x = ((unsigned long)e << 16) + (unsigned long)c * f + h;
    }
    if (x >= y)
    {
        x -= y;
    }
    return (x);
}


#endif

