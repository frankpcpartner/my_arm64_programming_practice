#ifndef _GPIO_H
#define _GPIO_H

#include "core.h"

#define GPIO_GPFSEL0_ADDR	(MMIO_BASE + 0x00200000)
#define GPIO_GPFSEL1_ADDR	(MMIO_BASE + 0x00200004)
#define GPIO_GPPUD_ADDR		(MMIO_BASE + 0x00200094)
#define GPIO_GPPUDCLK0_ADDR	(MMIO_BASE + 0x00200098)
#define GPIO_GPPUDCLK1_ADDR	(MMIO_BASE + 0x0020009C)


#define GPIO_FUNCTION_SELECT_MASK(i) 	(~(0x7 << ((i % 10) * 3 )))
#define GPIO_FUNCTION_SELECT_INPUT(i) 	(0x0 << ((i % 10) * 3 ))
#define GPIO_FUNCTION_SELECT_OUTPUT(i) 	(0x1 << ((i % 10) * 3 ))
#define GPIO_FUNCTION_SELECT_FUNC_0(i) 	(0x4 << ((i % 10) * 3 ))
#define GPIO_FUNCTION_SELECT_FUNC_1(i) 	(0x5 << ((i % 10) * 3 ))
#define GPIO_FUNCTION_SELECT_FUNC_2(i) 	(0x6 << ((i % 10) * 3 ))
#define GPIO_FUNCTION_SELECT_FUNC_3(i) 	(0x7 << ((i % 10) * 3 ))
#define GPIO_FUNCTION_SELECT_FUNC_4(i) 	(0x3 << ((i % 10) * 3 ))
#define GPIO_FUNCTION_SELECT_FUNC_5(i) 	(0x2 << ((i % 10) * 3 ))

#define GPIO_GPPUDCLK0_ASSERT_LINE(n)	(1<<n)  

#endif