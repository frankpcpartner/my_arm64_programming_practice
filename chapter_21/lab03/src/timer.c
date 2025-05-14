#include "timer.h"
#include "util.h"
#include "core.h"
#include "task.h"

static unsigned int arch_timer_rate = NSEC_PER_SEC / HZ;

unsigned long volatile cacheline_aligned jiffies = 0;

static void generic_timer_init();
static void generic_timer_reset(unsigned int val);
static void enable_timer_interrupt();

void timer_init()
{
	generic_timer_init();
	generic_timer_reset(arch_timer_rate);
	enable_timer_interrupt();
}

void handle_timer_irq()
{
	generic_timer_reset(arch_timer_rate);
	//printf("\rCore0 Timer interrupt received\n");
	jiffies++;

	task_handle_timer();
}

void generic_timer_init()
{
	register unsigned long long enable = 1;
	asm volatile("msr cntp_ctl_el0, %0"
		: 
		: "r"(enable)
		: "memory"
	);
}

static void generic_timer_reset(unsigned int val)
{
	asm volatile("msr cntp_tval_el0, %0"
		: 
		:"r"(val)
		:"memory"
	);
}

static void enable_timer_interrupt()
{
	unsigned int val = get(CORE0_TIMER_CNTRL_REG);

	printf("\r enable_timer_interrupt, val:%u\n",val);

    val |= TIMER_CNTRL_PNS_IRQ_ENABLE;    

    set(CORE0_TIMER_CNTRL_REG,val); 
}