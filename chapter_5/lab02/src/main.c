#include "uart.h"
#include "util.h"

extern int _test(unsigned long a, unsigned long b);

void main()
{
	uart_init(type_uart0);

	printf("\r Welcome BenOS!\n");

	_test(0,10);

	_test(1,10);
	
	while(1)
	{
		asm volatile("nop");
	}
}