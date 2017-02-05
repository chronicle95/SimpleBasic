#include "sbstruct.h"
#include "sbparse.h"
#include "sbmisc.h"

int get_symbol(const char *s, int i, char *t)
{
    while (s[i] && ISALPHA (s[i]))
    {
        *(t++) = s[i++];
    }
    *t = '\0';
    return i;
}

int get_number(const char *s, int i, int *num)
{
    *num = 0;
    while (s[i] && ISDIGIT (s[i]))
    {
        *num *= 10;
        *num += s[i++] - '0';
    }
    return i;
}

int ign_space(const char *s, int i)
{
    while (s[i] && ISSPACE (s[i]))
    {
        i++;
    }
    return i;
}

int seek_eol(const char *s, int i, int dir)
{
    while ((i >= 0) && s[i] && s[i] != '\n')
    {
        i += dir;
    }
    return i;
}

int seek_line(const char *s, int ln)
{
    int i = 0, x;
    while (s[i])
    {
        i = ign_space (s, i);
        i = get_number (s, i, &x);
        if (x == ln)
        {
            return i;
        }
        i = seek_eol (s, i, 1);
        i++;
    }
    return NO_LINE;
}

void upper(char *s)
{
    bool toggle = true;
    while (*s)
    {
        if (ISalpha (*s))
        {
            if (toggle)
                *s = *s - 32;
        }
        else
        if (*s == '\"')
        {
            toggle = !toggle;
        }
        s++;
    }
}
