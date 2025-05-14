#ifndef _CORE_H
#define _CORE_H

#define MMIO_BASE 			0x3F000000
#define MMIO_DEVICE_SIZE	0x2000000

#ifndef _ASM_

typedef int bool;
#define true 1
#define false 0

typedef unsigned long long uint64_t;

typedef unsigned int size_t;

typedef unsigned char byte;

typedef unsigned long long u64;
//typedef unsigned long long UL;

#define NULL ((void*)0)

#define offsetof(type, member) ((size_t) &((type *)0)->member)

#define BIT(nr)		((unsigned int)1 << (nr))

#define cacheline_aligned __attribute__((__aligned__(64)))

extern void set(unsigned long long address, unsigned int val);
extern unsigned int get(unsigned long long address);

#else



#endif

#endif