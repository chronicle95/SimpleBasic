#ifndef __SBSTRUCT_H__
#define __SBSTRUCT_H__

#include "sbconf.h"
#include "sbmisc.h"

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
    int line;
    bool running;
};

#endif
