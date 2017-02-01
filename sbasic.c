#include <stdio.h>

#define MAX_LINE 10000
#define NO_LINE -1

#define VAR_NAMESZ 10
#define VAR_COUNT 64
#define CMD_NAMESZ 10
#define CMD_COUNT 10
#define DATA_STSZ 16
#define CALL_STSZ 16
#define MEMORY_SZ 100
#define LINE_SZ 80
#define CODE_SZ 4096

/* Do not change the order of inclusion */
#include "sbmisc.c"
#include "sbstruct.c"
#include "sbparse.c"
#include "sbexec.c"

void list_program(const char *s)
{
    int line = 0, ci;
    while (line < MAX_LINE)
    {
        ci = seek_line (s, line);
        if (ci != NO_LINE)
        {
            printf ("%d ", line);
            ci = ign_space (s, ci);
            while (s[ci] && s[ci] != '\n')
            {
                putchar (s[ci]);
                ci++;
            }
            putchar ('\n');
        }
        line++;
    }
}

int main(int argc, char **argv)
{
    struct Context ctx;
    char prog[CODE_SZ];
    char linebuf[LINE_SZ];
    int linenum, x;
    
    printf ("SmallBasic\n");
    printf ("Ok\n");
    while (true)
    {
        stripped_input (linebuf);
        if (ISDIGIT (linebuf[0]))
        {
            x = get_number(linebuf, 0, &linenum);
            x = seek_line(prog, linenum);
            if (x == NO_LINE)
            {
                join (prog, "\n");
                join (prog, linebuf);
            }
            else
            {
                int prev_eol, next_eol, offset;
                prev_eol = seek_eol (prog, x, -1) + 1;
                next_eol = seek_eol (prog, x,  1);
                offset = length (linebuf) - (next_eol - prev_eol);
                shift (prog, next_eol, offset, CODE_SZ);
                copy (prog + prev_eol, linebuf);
            }
        }
        else
        if (ISALPHA (linebuf[0]))
        {
            if (compare (linebuf, "RUN"))
            {
                exec_program (prog, &ctx);
            }
            else
            if (compare (linebuf, "NEW"))
            {
                clear (prog, CODE_SZ);
            }
            else
            if (compare (linebuf, "LIST"))
            {
                list_program (prog);
            }
            else
            {
                printf ("Error: unknown command `%s`\n", linebuf);
            }
            printf ("Ok\n");
        }
        else
        {
            printf ("Error: unexpected character `%c`\n", linebuf[0]);
        }
    }
    
    return 0;
}
