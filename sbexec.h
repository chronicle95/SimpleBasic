#ifndef __SBEXEC_H__
#define __SBEXEC_H__

#include "sbstruct.h"

int exec_expr(const char *s, int i, struct Context *ctx);
void exec_line(const char *s, int i, struct Context *ctx);
void exec_program(const char *s, struct Context* ctx);

#endif
