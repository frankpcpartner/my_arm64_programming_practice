#ifndef _OFFSET_H
#define _OFFSET_H

#define DEFINE(sym, val) \
    printf("#define %-20s %ld\n", #sym, (unsigned long)(val));

#endif