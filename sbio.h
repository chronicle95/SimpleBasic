#ifndef __SBIO_H__
#define __SBIO_H__

void sbputc(char);
char sbgetc();
void sbputs(const char*);
void sbputn(int);
void sbprint(const char*, ...);

#endif
