#include "uart.h"
#include "util.h"
#include "timer.h"
#include "page.h"
#include "sysregs.h"
#include "cache.h"
#include "task.h"

extern unsigned long volatile cacheline_aligned jiffies;

extern char _text_boot_start, _text_boot_end;
extern char _text_start, _text_end;
extern char _rodata_start, _rodata_end;
extern char _data_start, _data_end;
extern char _bss_start, _bss_end;
extern char _test_ro_page_start[];
extern char _test_ro_page_end[];


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

static void print_el()
{
	long long el = 0;

	asm volatile(
		"mrs %[el], CurrentEL\n"
		"lsr %[el], %[el], #2"
		: [el]"+r"(el)
		:
		: "memory"
	);

	printf("\rel = %lld\n",el);
}


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

extern void trigger_alignment();

static inline void arch_local_irq_enable()
{
	asm volatile(
		"msr daifclr, #2"
		:
		:
		:"memory"
	);
}

static inline void arch_local_irq_disable()
{
	asm volatile(
		"msr daifset, #2"
		:
		:
		:"memory"
	);
}

static void test_access_mapping_address()
{
	unsigned long long address = TOTAL_MEMORY - 4096;
	*(unsigned long long*) address = 0x55;

	printf("\r%s access 0x%x done.\n",__func__,address);
}

static void test_access_unmapping_address()
{
	unsigned long long address = TOTAL_MEMORY + 4096;
	*(unsigned long long*) address = 0x55;

	printf("\r%s access 0x%x done.\n",__func__,address);
}

static void test_access_ro_page()
{
	pte_t* pte = walk_pgtable((unsigned long long)_test_ro_page_start);
	pte_t new_pte;

	if((pte_val(*pte) & PTE_RDONLY) != 0)
	{
		printf("\rAddress:0x%llx is read only.\n",(unsigned long long)_test_ro_page_start);
	}
	else
	{
		printf("\rAddress:0x%llx is not read only.\n",(unsigned long long)_test_ro_page_start);
	}

	pte_val(*pte) &= (~(PTE_RDONLY));

	asm volatile (
		"dsb ishst\n"
		"tlbi vmalle1\n"
		"dsb sy\n"
		"tlbi vmalle1\n"
	);


	*(unsigned long long*) _test_ro_page_start = 0x55;

	printf("\r%s access 0x%x done.\n",__func__,(unsigned long long)_test_ro_page_start);
}

extern void my_atomic_write();
extern char my_atomic_data[];
extern bool test_at_instruction_asm(unsigned long long address);

void test_at_instruction()
{
	unsigned long long address = TOTAL_MEMORY - 4096;

	if(test_at_instruction_asm(address))
	{
		printf("\rtest AT instruction done\n");
	}
	else
	{
		printf("\rtest AT instruction fail\n");
	}

	address = TOTAL_MEMORY + 4096;

	if(test_at_instruction_asm(address))
	{
		printf("\rtest AT instruction done\n");
	}
	else
	{
		printf("\rtest AT instruction fail\n");
	}
}


void print_at_result(unsigned long long address, unsigned long long par_el1)
{
	if((par_el1 & PAR_EL1_FAULT_FAILED) == PAR_EL1_FAULT_FAILED)
	{
		printf("\rAddress translation failed\n");
		return;
	}

	printf("\rAddress translation successfully\n");

	unsigned long long pa = (par_el1 & PAR_EL1_PA_MASK) | (address & 0xfff);

	printf("\rparel1:0x%llx, PA: 0x%llx\n",par_el1,pa);
	printf("\r NS=%d\n",PAR_EL1_NS(par_el1));
	printf("\r SH=%d\n",PAR_EL1_SH(par_el1));
	printf("\r ATTR=0x%x\n",PAR_EL1_ATTR(par_el1));
}

#define NOOP 10000
static void test_cache(void)
{
	unsigned long start = jiffies;

	int i, j;
	char var;
	unsigned long long bss_start = (unsigned long long)&_bss_start;
	unsigned long long bss_end = (unsigned long long)&_bss_end;
	unsigned long long bss_size = bss_end - bss_start;
	char* bss = &_bss_start;

	show_cache_info();

	for (j = 0; j < NOOP; j++) 
	{
		flush_cache_range(bss_start, bss_end -1);		
    
		for(i = 0; i < bss_size; i++)
		{
			var = *(bss + i);
		}
	}

	printf("\r%s time %llu jiffies\n", __func__, jiffies - start);	
}

static void delay(int n)
{
	while (n--)
		;
}

void do_fork_test_fn()
{
	while (1) {
		delay(10000000);
		printf("\r%s %d\n",__func__,12345);
	}	
}

void test_do_fork()
{
	int pid = do_fork(PF_KTHREAD,(unsigned long long)&do_fork_test_fn,0);

	if(pid < 0)
	{
		printf("\rtest_do_fork failed.\n");
		return;
	}

	
	switch_to(g_task[pid]);
}

void main()
{
	uart_init(type_uart0);

	printf("\rWelcome BenOS!\n");

	_test();


	print_func_name(0x800860);
	print_func_name(0x800800);
	print_func_name(0x800880);
	print_func_name(0x800000);

	print_image_layout();

	my_memcpy((void*) 0x80000, (void*) 0x100000, 32);

	print_el();

	//trigger_alignment();

	//page_init();

	//page_dump();

	test_access_mapping_address();
	//test_access_unmapping_address();

	test_access_ro_page();
	
	my_atomic_write();

	test_at_instruction();

	timer_init();
	arch_local_irq_enable();
	
	test_do_fork();

	//test_cache();

	while(1)
	{
		asm volatile("nop");
	}
}