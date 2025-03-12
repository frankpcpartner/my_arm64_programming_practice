#include "core.h"

void set(unsigned long long address, unsigned int val)
{
	volatile unsigned int* reg = (volatile unsigned int*) address;
	(*reg) = val;
}

unsigned int get(unsigned long long address)
{
	volatile unsigned int* reg = (volatile unsigned int*) address;
	return (*reg);
}