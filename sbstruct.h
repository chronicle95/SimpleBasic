#ifndef __SBSTRUCT_H__
#define __SBSTRUCT_H__

#include "sbconf.h"
#include "sbbool.h"

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

struct Context
{
    struct Variable vars[VAR_COUNT];
    struct Command cmds[CMD_COUNT];
    int dmemory[MEMORY_SZ];
    int dstack[DATA_STSZ];
    int cstack[CALL_STSZ];
    int dsptr;
    int csptr;
    int allocated;
    int line;
    int top_line;
    char error;
#define ERR_NONE     0
#define ERR_UNDEF   -1
#define ERR_UNEXP   -2
#define ERR_UNKNOWN -3
    bool running;
};

#endif
