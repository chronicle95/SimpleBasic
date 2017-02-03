#ifndef __SBEXEC_H__
#define __SBEXEC_H__

#include "sbmisc.h"
#include "sbstruct.h"

#define BINDCMD(c,n,f)  do {\
                        copy ((c)->name, n);\
                        (c)->func = f;\
                    } while(0)

int exec_expr(const char *s, int i, struct Context *ctx);
void exec_line(const char *s, int i, struct Context *ctx);
void exec_program(const char *s, struct Context* ctx);
void exec_init(struct Context *ctx);

void exec_cmd_print(const char *s, int i, struct Context *ctx);
void exec_cmd_input(const char *s, int i, struct Context *ctx);
void exec_cmd_return(const char *s, int i, struct Context *ctx);
void exec_cmd_goto(const char *s, int i, struct Context *ctx);
void exec_cmd_gosub(const char *s, int i, struct Context *ctx);
void exec_cmd_let(const char *s, int i, struct Context *ctx);
void exec_cmd_end(const char *s, int i, struct Context *ctx);
void exec_cmd_stop(const char *s, int i, struct Context *ctx);
void exec_cmd_if(const char *s, int i, struct Context *ctx);

#endif
