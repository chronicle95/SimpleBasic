#include <stdio.h>
#include <stdarg.h>

#include "sbio.h"

char sbgetc()
{
    return getchar ();
}

void sbputc(char c)
{
    putchar (c);
}

void sbputs(const char *s)
{
    while (*s)
    {
        sbputc (*s);
        s++;
    }
}

void sbputn(int n)
{
    int i = 1, t = n;
    while (t)
    {
        t /= 10;
        i *= 10;
    }
    while (i > 1)
    {
        i /= 10;
        sbputc (n/i%10 + '0');
    }
}

void sbprint(const char *fmt, ...)
{
    va_list args;
    va_start (args, fmt);
    while (*fmt)
    {
        if (*fmt == '%')
        {
            fmt++;
            switch (*fmt)
            {
                case 'c':
                    sbputc (va_arg (args, int));
                    break;
                case 'i':
                case 'd':
                    sbputn (va_arg (args, int));
                    break;
                case 's':
                    sbputs (va_arg (args, const char*));
                    break;
            }
        }
        else
        {
            sbputc (*fmt);
        }
        fmt++;
    }
    va_end (args);
}
