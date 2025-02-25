#include "uart.h"
#include "util.h"

extern void _test(void);

void main()
{
	uart_init(type_uart0);

	printf("\r Welcome BenOS!\n");

	_test();


	while(1)
	{
		asm volatile("nop");
	}
}