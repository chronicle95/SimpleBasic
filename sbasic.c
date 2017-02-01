#include <stdio.h>

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

#include "sbmisc.c"

/* structures */
struct Variable
{
    char name[VAR_NAMESZ];
    int location;
};

struct Context;
struct Command
{
    char name[CMD_NAMESZ];
    void (*func)(const char*, int, struct Context*);
};
#define BINDCMD(c,n,f)  do {\
                        copy ((c)->name, n);\
                        (c)->func = f;\
                    } while(0);

struct Context
{
    struct Variable vars[VAR_COUNT];
    struct Command cmds[CMD_COUNT];
    int dmemory[MEMORY_SZ];
    int dstack[DATA_STSZ];
    int cstack[CALL_STSZ];
    int dsptr;
    int csptr;
    int line;
    bool running;
};

/* interpreter functions */
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

int exec_expr(const char *s, int i, struct Context *ctx)
{
    char name[VAR_NAMESZ];
    char pending = '=';
    bool operand;
    int value, j;
    while (s[i] && (s[i] != ';') && (s[i] != '\n'))
    {
        operand = false;
        i = ign_space (s, i);
        if (ISDIGIT (s[i]))
        {
            i = get_number (s, i, &value);
            operand = true;
        }
        else
        if (ISALPHA (s[i]))
        {
            i = get_symbol (s, i, name);
            if (compare (name, "THEN"))
            {
                break;
            }
            for (j = 0; j < VAR_COUNT; j++)
            {
                if (compare (ctx->vars[j].name, name))
                {
                    value = ctx->dmemory[ctx->vars[j].location];
                    operand = true;
                    break;
                }
            }
            if (!operand)
            {
                printf ("Error: undefined variable `%s` at line %d\n",
                        name, ctx->line);
                ctx->running = false;
                break;
            }
        }
        else
        if (ISOP (s[i]))
        {
            pending = s[i++];
        }
        else
        {
            printf ("Error: unexpected character `%c` at line %d\n",
                    s[i], ctx->line);
            ctx->running = false;
            break;
        }
        
        /* process pending operation with the operand value */
        if (operand)
        {
            switch (pending)
            {
                case '=':
                    if (ctx->dsptr == 0)
                    {
                        ctx->dsptr++;
                        ctx->dstack[ctx->dsptr] = value;
                    }
                    else
                    {
                        ctx->dstack[ctx->dsptr] = (ctx->dstack[ctx->dsptr] == value);
                    }
                    break;
                case '+':
                    ctx->dstack[ctx->dsptr] += value;
                    break;
                case '-':
                    ctx->dstack[ctx->dsptr] -= value;
                    break;
                case '*':
                    ctx->dstack[ctx->dsptr] *= value;
                    break;
                case '/':
                    ctx->dstack[ctx->dsptr] /= value;
                    break;
                case '>':
                    ctx->dstack[ctx->dsptr] = ctx->dstack[ctx->dsptr] > value;
                    break;
                case '<':
                    ctx->dstack[ctx->dsptr] = ctx->dstack[ctx->dsptr] < value;
                    break;
            }
        }
    }
    return i;
}

void exec_line(const char *s, int i, struct Context *ctx)
{
    int j;
    char cmd[CMD_NAMESZ];
    i = ign_space (s, i);
    i = get_symbol (s, i, cmd);
    for (j = 0; j < CMD_COUNT; j++)
    {
        if (compare (cmd, ctx->cmds[j].name))
        {
            ctx->cmds[j].func (s, i, ctx);
            return;
        }
    }
    ctx->running = false;
    printf("Error: unknown command `%s`at line %d\n", cmd, ctx->line);
}

void exec_program(const char *s, struct Context* ctx)
{
    int ci;
    ctx->running = true;
    ctx->line    = 0;
    ctx->dsptr   = 0;
    ctx->csptr   = 0;
    
    while (ctx->running && (ctx->line < MAX_LINE))
    {
        ci = seek_line (s, ctx->line);
        if (ci != NO_LINE)
        {
            exec_line (s, ci, ctx);
        }
        ctx->line++;
    }
}

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
