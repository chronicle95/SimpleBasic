#ifndef __SBCONF_H__
#define __SBCONF_H__

#define MAX_LINE 10000
#define NO_LINE -1

#define VAR_NAMESZ 10       /* maximum variable name length */
#define VAR_COUNT 64        /* variable limit */
#define CMD_NAMESZ 10       /* limit for command words */
#define CMD_COUNT 10        /* number of available commands */
#define DATA_STSZ 16        /* depth of calculation */
#define CALL_STSZ 16        /* subroutine call depth */
#define MEMORY_SZ 100       /* data memory size */
#define LINE_SZ 80          /* line width restriction */
#define CODE_SZ 4096        /* program size in characters */

#define FILE_SUPPORT 1   /* 0 - no, 1 - yes */

#endif
