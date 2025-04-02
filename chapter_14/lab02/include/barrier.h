#ifndef _BARRIER_H
#define _BARRIER_H

#ifndef _ASM_
#define isb()		asm volatile("isb" : : : "memory")
#define dmb(opt)	asm volatile("dmb " #opt : : : "memory")
#define dsb(opt)	asm volatile("dsb " #opt : : : "memory")
#endif



#endif