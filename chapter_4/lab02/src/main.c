#include "uart.h"
#include "util.h"

extern unsigned long long _test(unsigned long a, unsigned long b);

void main()
{
	uart_init(type_uart0);

	printf("\r Welcome BenOS!\n");

	unsigned long a = 1;
	unsigned long b = 2;
	unsigned long long r = _test(a,b);
	printf("\r a:%u,b:%u,r:%llu\n",a,b,r);

	a = 2;
	r = _test(a,b);
	printf("\r a:%u,b:%u,r:%llu\n",a,b,r);

	a = 10;
	r = _test(a,b);
	printf("\r a:%u,b:%u,r:%llu\n",a,b,r);


	while(1)
	{
		asm volatile("nop");
	}
}