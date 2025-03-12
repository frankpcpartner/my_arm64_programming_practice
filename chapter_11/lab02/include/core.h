#ifndef _CORE_H
#define _CORE_H

#define MMIO_BASE 0x3F000000

#ifndef _ASM_

typedef int bool;
#define true 1
#define false 0

typedef unsigned long long uint64_t;

typedef unsigned int size_t;

typedef unsigned char byte;

#define NULL ((void*)0)



extern void set(unsigned long long address, unsigned int val);
extern unsigned int get(unsigned long long address);

#endif

#endif