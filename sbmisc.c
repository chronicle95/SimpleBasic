#include "sbconf.h"
#include "sbparse.h"
#include "sbmisc.h"
#include "sbio.h"

bool compare(const char *a, const char *b)
{
    if (a == false && b == false) return true;
    if (a == false || b == false) return false;
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
        c = sbgetc ();
    }
    while (ISSPACE(c));
    
    while (c != '\n')
    {
        *(s++) = c;
        c = sbgetc ();
    }
    *s = '\0';
}

void map_intval (struct Context *ctx, const char *key, int value, int size)
{
    int j;
    for (j = 0; ctx->vars[j].name[0] && j < VAR_COUNT; j++)
    {
        if (compare (ctx->vars[j].name, key))
        {
            ctx->dmemory[ctx->vars[j].location] = value;
            return;
        }
    }
    copy (ctx->vars[j].name, key);
    ctx->vars[j].location = ctx->allocated;
    ctx->allocated += size;
    for (j = 0; j < size; j++)
    {
        ctx->dmemory[ctx->vars[j].location + j] = value;
    }
}
