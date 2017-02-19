#include "sbconf.h"
#include "sbparse.h"
#include "sbmisc.h"
#include "sbexec.h"
#include "sbio.h"

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
                sbprint ("Error: undefined variable `%s` at line %d\n",
                        name, ctx->line);
                ctx->running = false;
                ctx->error = ERR_UNDEF;
                break;
            }
        }
        else
        if (ISOP (s[i]))
        {
            if (s[i] == '(')
            {
                ctx->dsptr++;
                ctx->dstack[ctx->dsptr] = (int) pending;
                pending = s[i++];
            }
            else
            if (s[i] == ')')
            {
                value = ctx->dstack[ctx->dsptr--];
                pending = (char) ctx->dstack[ctx->dsptr--];
                operand = true;
                i++;
            }
            else
            {
                pending = s[i++];
            }
        }
        else
        {
            sbprint ("Error: unexpected character `%c` at line %d\n",
                    s[i], ctx->line);
            ctx->running = false;
            ctx->error = ERR_UNEXP;
            i++;
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
                case '(':
                    ctx->dsptr++;
                    ctx->dstack[ctx->dsptr] = value;
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
    if (s[i] == '\'')
    {
        /* Don't process a remark */
        return;
    }
    else
    if (!ISALPHA (s[i]))
    {
        ctx->running = false;
        ctx->error = ERR_UNEXP;
        sbprint ("Error: unexpected character `%c` at line %d\n", s[i], ctx->line);
        return;
    }
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
    ctx->error = ERR_UNKNOWN;
    sbprint ("Error: unknown command `%s` at line %d\n", cmd, ctx->line);
}

void exec_program(const char *s, struct Context* ctx)
{
    int ci;
    ctx->running = true;
    ctx->line    = 0;
    ctx->dsptr   = 0;
    ctx->csptr   = 0;
    ctx->allocated = 0;
    ctx->error   = ERR_NONE;
    
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


void exec_cmd_print(const char *s, int i, struct Context *ctx)
{
    bool eol = true;
    while ((i < length (s)) && (s[i] != '\n'))
    {
        i = ign_space (s, i);
        if (s[i] == '\"')
        {
            i++;
            while (s[i] != '\"')
            {
                sbputc (s[i++]);
            }
            /* ignore closing quotation mark */
            i++;
            eol = true;
        }
        else
        if (s[i] == ';')
        {
            i++;
            eol = false;
        }
        else
        {
            i = exec_expr (s, i, ctx);
            if (ctx->error == ERR_NONE)
            {
                sbprint ("%d ", ctx->dstack[ctx->dsptr--]);
            }
            eol = true;
        }
    }
    if (eol) sbputc ('\n');
}

void exec_cmd_input(const char *s, int i, struct Context *ctx)
{
    char name[VAR_NAMESZ], ch;
    int value = 0, j;

    i = ign_space (s, i);
    if (s[i] == '\"')
    {
        i++;
        while (s[i] != '\"')
        {
            sbputc (s[i]);
            i++;
        }
        /* ignore quotation mark */
        i++;
    }
    i = ign_space (s, i);
    i = get_symbol (s, i, name);

    ch = sbgetc ();
    while (ISDIGIT (ch))
    {
        value *= 10;
        value += ch - '0';
        ch = sbgetc ();
    }

    map_intval (ctx, name, value);
}

void exec_cmd_return(const char *s, int i, struct Context *ctx)
{
    ctx->line = ctx->cstack[ctx->csptr--];
}

void exec_cmd_goto(const char *s, int i, struct Context *ctx)
{
    exec_expr (s, i, ctx);
    /* subtract one from line number because
     * it will be anyways incremented in main execution
     * loop */
    ctx->line = ctx->dstack[ctx->dsptr--] - 1;
}

void exec_cmd_gosub(const char *s, int i, struct Context *ctx)
{
    exec_expr (s, i, ctx);
    ctx->cstack[++ctx->csptr] = ctx->line;
    ctx->line = ctx->dstack[ctx->dsptr--] - 1;
}

void exec_cmd_let(const char *s, int i, struct Context *ctx)
{
    char name[VAR_NAMESZ];
    int j;
    i = ign_space (s, i);
    i = get_symbol (s, i, name);
    exec_expr (s, i, ctx);
    map_intval (ctx, name, ctx->dstack[ctx->dsptr--]);
}

void exec_cmd_end(const char *s, int i, struct Context *ctx)
{
    ctx->running = false;
}

void exec_cmd_if(const char *s, int i, struct Context *ctx)
{
    i = exec_expr (s, i, ctx);
    if (ctx->dstack[ctx->dsptr--])
    {
        exec_line (s, i, ctx);
    }
}

void exec_init(struct Context *ctx)
{
    BINDCMD (&ctx->cmds[0], "PRINT",  exec_cmd_print);
    BINDCMD (&ctx->cmds[1], "INPUT",  exec_cmd_input);
    BINDCMD (&ctx->cmds[2], "RETURN", exec_cmd_return);
    BINDCMD (&ctx->cmds[3], "GOTO",   exec_cmd_goto);
    BINDCMD (&ctx->cmds[4], "GOSUB",  exec_cmd_gosub);
    BINDCMD (&ctx->cmds[5], "LET",    exec_cmd_let);
    BINDCMD (&ctx->cmds[6], "END",    exec_cmd_end);
    BINDCMD (&ctx->cmds[7], "STOP",   exec_cmd_end);
    BINDCMD (&ctx->cmds[8], "IF",     exec_cmd_if);
}
