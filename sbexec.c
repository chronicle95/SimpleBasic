#include <stdio.h>

#include "sbconf.h"
#include "sbparse.h"
#include "sbmisc.h"
#include "sbexec.h"

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

