#ifndef __SBMISC_H__
#define __SBMISC_H__

#define true 1
#define false 0
typedef char bool;

bool compare(const char *a, const char *b);
void copy(char *dst, const char *src);
void clear(char *dst, int size);
int length(const char *s);
void join(char *dst, const char *src);
void shift(char *s, int origin, int offset, int maxlength);
void stripped_input(char *s);

#endif
