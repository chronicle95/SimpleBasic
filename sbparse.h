#ifndef __SBPARSE_H__
#define __SBPARSE_H__

#define ISDIGIT(c) (((c)>='0')&&((c)<='9'))
#define ISALPHA(c) (((c)>='A')&&((c)<='Z'))
#define ISSPACE(c) (((c)==' ')||((c)=='\t'))
#define ISOP(c) (((c)=='+')||((c)=='-')||((c)=='*')\
		                ||((c)=='/')||((c)=='>')||((c)=='<')\
		                ||((c)=='='))

int get_symbol(const char *s, int i, char *t);
int get_number(const char *s, int i, int *num);
int ign_space(const char *s, int i);
int seek_eol(const char *s, int i, int dir);
int seek_line(const char *s, int ln);

#endif
