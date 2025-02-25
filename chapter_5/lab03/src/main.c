#include "uart.h"
#include "util.h"

extern void bl_test();

void main()
{
	uart_init(type_uart0);

	printf("\r Welcome BenOS!\n");

	bl_test();
	
	while(1)
	{
		asm volatile("nop");
	}
}