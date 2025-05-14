#include "irq.h"


void raw_local_irq_enable()
{
	asm volatile(
		"msr daifclr, #2"
		:
		:
		:"memory"
	);
}

void raw_local_irq_disable()
{
	asm volatile(
		"msr daifset, #2"
		:
		:
		:"memory"
	);	
}

