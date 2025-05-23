#ifndef _TIMER_H
#define _TIMER_H

#include "core.h"

#define GENERIC_TIMER_PERIOD 	0xFFFFFFF

#define CORE0_TIMER_CNTRL_REG		0x40000040
#define TIMER_CNTRL_PNS_IRQ_ENABLE 	(0b10)

#define HZ 250
#define NSEC_PER_SEC    1000000000L

void timer_init();
void handle_timer_irq();

#endif