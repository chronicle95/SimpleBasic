
/* boolean type definitions */
#define true 1
#define false 0
typedef char bool;

/* global constants and macros */
#define ISDIGIT(c) (((c)>='0')&&((c)<='9'))
#define ISALPHA(c) (((c)>='A')&&((c)<='Z'))
#define ISSPACE(c) (((c)==' ')||((c)=='\t'))
#define ISOP(c) (((c)=='+')||((c)=='-')||((c)=='*')\
                ||((c)=='/')||((c)=='>')||((c)=='<')\
                ||((c)=='='))

/* string and i/o related methods */
bool compare(const char *a, const char *b)
{
    if (a == NULL && b == NULL) return true;
    if (a == NULL || b == NULL) return false;
    while (*a && *b && (*a == *b))
    {
        a++;
        b++;
    }
    if ((*a == '\0') && (*b == '\0')) return true;
    return false;
}

void copy(char *dst, const char *src)
{
    while (*src)
    {
        *(dst++) = *(src++);
    }
}

void clear(char *dst, int size)
{
    for (; size>0; size--, dst++)
    {
        *dst = 0;
    }
}

int length(const char *s)
{
    int l = 0;
    while (*(s++)) l++;
    return l;
}

void join(char *dst, const char *src)
{
    copy (dst + length (dst), src);
}

void shift(char *s, int origin, int offset, int maxlength)
{
    int i;
    if (offset > 0)
    {
        i = maxlength - 1;
        while ((i - offset) >= origin)
        {
            *(s + i) = *(s + i - offset);
            i--;
        }
        while (--i >= origin)
        {
            *(s + i) = 0;
        }
    }
    else if (offset < 0)
    {
        i = origin + offset;
        while (*(s + i - offset))
        {
            *(s + i) = *(s + i - offset);
            i++;
        }
        while (i < maxlength)
        {
            *(s + i) = 0;
            i++;
        }
    }
}

void stripped_input(char *s)
{
    char c;
    do
    {
        c = getchar();
    }
    while (ISSPACE(c));
    
    while (c != '\n')
    {
        *(s++) = c;
        c = getchar();
    }
    *s = '\0';
}


