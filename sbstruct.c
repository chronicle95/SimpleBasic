
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
