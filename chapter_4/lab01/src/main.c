#include "uart.h"
#include "util.h"

extern void _test1();
extern void _test2();

void main()
{
	uart_init(type_uart0);

	printf("\r Welcome BenOS!\n");

	_test1();
	_test2();

	while(1)
	{
		asm volatile("nop");
	}
}