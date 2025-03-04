#include "uart.h"
#include "util.h"

extern void _test();

void main()
{
	uart_init(type_uart0);

	printf("\rWelcome BenOS!\n");

	_test();
	
	while(1)
	{
		asm volatile("nop");
	}
}