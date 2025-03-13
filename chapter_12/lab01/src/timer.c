#include "timer.h"
#include "util.h"

static void generic_timer_init();
static void generic_timer_reset(unsigned int val);
static void enable_timer_interrupt();

void timer_init()
{
	generic_timer_init();
	generic_timer_reset(GENERIC_TIMER_PERIOD);
	enable_timer_interrupt();
}

void handle_timer_irq()
{
	generic_timer_reset(GENERIC_TIMER_PERIOD);
	printf("\rCore0 Timer interrupt received\n");
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
    val |= TIMER_CNTRL_PNS_IRQ_ENABLE;    

    set(CORE0_TIMER_CNTRL_REG,val); 
}