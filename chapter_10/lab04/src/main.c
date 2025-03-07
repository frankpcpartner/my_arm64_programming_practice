#include "uart.h"
#include "util.h"

extern char _text_boot_start, _text_boot_end;
extern char _text_start, _text_end;
extern char _rodata_start, _rodata_end;
extern char _data_start, _data_end;
extern char _bss_start, _bss_end;

void print_image_layout()
{
	long long text_boot_start = (long long)&_text_boot_start;
	long long text_boot_end = (long long)&_text_boot_end;
	long long text_boot_size = text_boot_end - text_boot_start;

	long long text_start = (long long)&_text_start;
	long long text_end = (long long)&_text_end;
	long long text_size = text_end - text_start;

	long long rodata_start = (long long)&_rodata_start;
	long long rodata_end = (long long)&_rodata_end;
	long long rodata_size = rodata_end - rodata_start;

	long long data_start = (long long)&_data_start;
	long long data_end = (long long)&_data_end;
	long long data_size = data_end - data_start;

	long long bss_start = (long long)&_bss_start;
	long long bss_end = (long long)&_bss_end;
	long long bss_size = bss_end - bss_start;

	printf("\rBenOS image layout:\n");
	printf("\r .text.boot: 0x%x - 0x%x ( %lld B)\n", text_boot_start,text_boot_end, text_boot_size);
	printf("\r      .text: 0x%x - 0x%x ( %lld B)\n", text_start,text_end, text_size);
	printf("\r    .rodata: 0x%x - 0x%x ( %lld B)\n", rodata_start,rodata_end, rodata_size);
	printf("\r      .data: 0x%x - 0x%x ( %lld B)\n", data_start,data_end, data_size);
	printf("\r       .bss: 0x%x - 0x%x ( %lld B)\n", bss_start,bss_end, bss_size);	
}

#if 0
void my_memcpy(void* src, void* dst,int size)
{
	if(size <= 0 || size % 8 != 0)
		return;

	asm volatile(
		"1: ldr x0, [%0], #8\n"
		"	str x0, [%1], #8\n"
		"	sub %2, %2, #8\n"
		"	cbnz %2, 1b\n"
		:"+r" (src), "+r" (dst), "+r" (size)
		:
		: "memory","x0"
	);
}
#else
void my_memcpy(void* src, void* dst,int size)
{
	if(size <= 0 || size % 8 != 0)
		return;

	asm volatile(
		"1: ldr x0, [%[src]], #8\n"
		"	str x0, [%[dst]], #8\n"
		"	sub %[size], %[size], #8\n"
		"	cbnz %[size], 1b\n"
		:[src]"+r" (src), [dst]"+r" (dst), [size]"+r" (size)
		:
		: "memory","x0"
	);
}
#endif

void __memset_16bytes(void* dst, long long data, int count)
{
	if(count <= 0 || count % 16 != 0)
		return;

	long long end = (long long)dst + count;

	asm volatile(
		"1: stp %[data], %[data], [%[dst]], #16\n"
		"	cmp %[dst], %[end]\n"
		"	bne 1b\n"
		:[dst]"+r"(dst)
		:[end]"r"(end),[data]"r"(data)
		:"memory"
	);
}

#define MY_OPS(op, instruction)							\
static inline void my_ops_##op(int i, void* addr) 		\
{														\
														\
	asm volatile( 										\
		" ldr x2, [%[addr]]\n" 							\
		" "#instruction" x2, x2, %[i]\n" 					\
		" str x2, [%[addr]]\n" 							\
		:[i] "+r"(i), [addr]"+r"(addr) 					\
		: 												\
		:"memory","x2" 									\
		);												\
}

MY_OPS(or, orr)
MY_OPS(xor, eor)
MY_OPS(and, and)
MY_OPS(andnot, bic)

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

	print_image_layout();

	my_memcpy((void*) 0x80000, (void*) 0x100000, 32);

	__memset_16bytes((void*)0x200000,0xabababababababab,64);

	long long test_num = 0x12340000;
	int i = 10;

	printf("\rBefore calling my_ops_or,i:%d,test_num:0x%x\n",i,test_num);
	my_ops_or(i,&test_num);
	printf("\rAfter calling my_ops_or,i:%d,test_num:0x%x\n",i,test_num);

	i = 123;
	printf("\rBefore calling my_ops_xor,i:%d,test_num:0x%x\n",i,test_num);
	my_ops_xor(i,&test_num);
	printf("\rAfter calling my_ops_xor,i:%d,test_num:0x%x\n",i,test_num);

	i = 0x111;
	printf("\rBefore calling my_ops_and,i:%d,test_num:0x%x\n",i,test_num);
	my_ops_and(i,&test_num);
	printf("\rAfter calling my_ops_and,i:%d,test_num:0x%x\n",i,test_num);

	i = 0x1;
	printf("\rBefore calling my_ops_andnot,i:%d,test_num:0x%x\n",i,test_num);
	my_ops_andnot(i,&test_num);
	printf("\rAfter calling my_ops_andnot,i:%d,test_num:0x%x\n",i,test_num);


	while(1)
	{
		asm volatile("nop");
	}
}