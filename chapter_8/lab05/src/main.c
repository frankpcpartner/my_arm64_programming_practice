#include "uart.h"
#include "util.h"

extern void _test();

extern int fun_num;
extern char fun_name[];
extern long long fun_addr[];

void print_func_name(long long addr)
{
	int i = 0;
	char* p = NULL, *string = NULL;

	for(; i < fun_num; i++)
	{
		if(addr == fun_addr[i])
		{
			break;
		}
	}

	if(i >= fun_num)
	{	
		printf("\rFunction with addr:0x%x not found\n",addr);
		return;
	}

	p = fun_name;

	if(i == 0)
	{
		string = p;
	}
	else
	{
		while(1)
		{
			if(*p == '\0')
			{
				i--;
			}

			if(i == 0)
			{
				string = p + 1;
				break;
			}

			p++;
		}
	}

	printf("\r%s\n",string);
}


void main()
{
	uart_init(type_uart0);

	printf("\r Welcome BenOS!\n");

	_test();
	
	print_func_name(0x800860);
	print_func_name(0x800800);
	print_func_name(0x800880);
	print_func_name(0x800000);

	while(1)
	{
		asm volatile("nop");
	}
}