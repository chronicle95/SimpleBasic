#include "sbconf.h"
#include "sbmisc.h"
#include "sbstruct.h"
#include "sbparse.h"
#include "sbexec.h"
#include "sbio.h"

void list_program(const char *s, int top)
{
    int line = 0, ci;
    while (line <= top)
    {
        ci = seek_line (s, line);
        if (ci != NO_LINE)
        {
            sbprint ("%d ", line);
            ci = ign_space (s, ci);
            while (s[ci] && s[ci] != '\n')
            {
                sbputc (s[ci]);
                ci++;
            }
            sbputc ('\n');
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
 
    clear (prog, CODE_SZ);
    clear ((char *) &ctx, sizeof(ctx));
    exec_init (&ctx);

    sbprint ("SimpleBasic\n");
    sbprint ("Ok\n");
    while (true)
    {
        stripped_input (linebuf);
        upper (linebuf);
        /* process line of code */
        if (ISDIGIT (linebuf[0]))
        {
            x = get_number(linebuf, 0, &linenum);
            if (linenum > MAX_LINE)
            {
                sbprint ("Error: line number should be less than %d\n",
                        MAX_LINE);
                continue;
            }

            x = seek_line(prog, linenum);
            if (x == NO_LINE)
            {
                if (length (prog))
                {
                    join (prog, "\n");
                }
                join (prog, linebuf);

                if (linenum > ctx.top_line)
                {
                    ctx.top_line = linenum;
                }
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
        /* process interpreter command */
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
                clear ((char *) &ctx, sizeof(ctx));
                exec_init (&ctx);
            }
            else
            if (compare (linebuf, "LIST"))
            {
                list_program (prog, ctx.top_line);
            }
            else
            if (compare (linebuf, "SYSTEM"))
            {
                break;
            }
            else
            {
                sbprint ("Error: unknown command `%s`\n", linebuf);
            }
            sbprint ("Ok\n");
        }
        else
        /* ignore empty line */
        if (ISSPACE (linebuf[0]) || !linebuf[0])
        {
            continue;
        }
        else
        {
            sbprint ("Error: unexpected character `%c`\n", linebuf[0]);
        }
    }
    
    return 0;
}
