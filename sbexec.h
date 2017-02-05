#ifndef __SBEXEC_H__
#define __SBEXEC_H__

#include "sbmisc.h"
#include "sbstruct.h"

#define BINDCMD(c,n,f)  do {\
                        copy ((c)->name, n);\
                        (c)->func = f;\
                    } while(0)

void exec_program(const char *s, struct Context* ctx);
void exec_init(struct Context *ctx);

#endif
