#include <stdio.h>

extern unsigned int my_atomic_write(unsigned int data);
extern void atomic_set(int nr, volatile unsigned long* addr);

int main()
{
	unsigned int p1 = my_atomic_write(0x34);
	unsigned long test = 0;

	atomic_set(10,&test);

	printf("Test, 0x%x,0x%lx\r\n",p1, test);
	return 0;
}