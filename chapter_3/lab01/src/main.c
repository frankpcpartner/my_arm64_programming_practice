#include "uart.h"
#include "util.h"



void main()
{
	uart_init(type_uart0);

	printf("\r Welcome BenOS!\n");

	while(1)
	{
		asm volatile("nop");
	}
}