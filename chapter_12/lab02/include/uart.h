#ifndef _UART_H
#define _UART_H

#include "core.h"

#define AUX_ENABLES_ADDR		(MMIO_BASE + 0x00215004) 
#define AUX_MU_IO_REG_ADDR		(MMIO_BASE + 0x00215040)
#define AUX_MU_IER_REG_ADDR 	(MMIO_BASE + 0x00215044)
#define AUX_MU_IIR_REG_ADDR		(MMIO_BASE + 0x00215048)		  	
#define AUX_MU_LCR_REG_ADDR		(MMIO_BASE + 0x0021504C)		  	
#define AUX_MU_MCR_REG_ADDR		(MMIO_BASE + 0x00215050)
#define AUX_MU_LSR_REG_ADDR		(MMIO_BASE + 0x00215054)
#define AUX_MU_BAUD_REG_ADDR	(MMIO_BASE + 0x00215068)		  
#define AUX_MU_CNTL_REG_ADDR	(MMIO_BASE + 0x00215060)		  

#define AUX_ENABLES_UART_ENABLE		(1 << 0)
#define AUX_ENABLES_UART_MASK		(~(1 << 0))

#define AUX_MU_IER_REG_TX_INTR_ENABLE	(1 << 1)
#define AUX_MU_IER_REG_TX_INTR_MASK		(~(1 << 1))

#define AUX_MU_IER_REG_RX_INTR_ENABLE	(1 << 0)
#define AUX_MU_IER_REG_RX_INTR_MASK		((1 << 0))

#define AUX_MU_LCR_REG_7BIT_MODE		(0x0)
#define AUX_MU_LCR_REG_8BIT_MODE		(0x3)
#define AUX_MU_LCR_REG_BIT_MODE_MASK	(~0x3)

#define AUX_MU_LSR_REG_TX_EMPTY		(1 << 5)
#define AUX_MU_LSR_REG_RX_READY		(1 << 0)


#define AUX_MU_CNTL_REG_TX_ENABLE	(1 << 1)
#define AUX_MU_CNTL_REG_TX_MASK		(~(1 << 1))

#define AUX_MU_CNTL_REG_RX_ENABLE	(1 << 0)
#define AUX_MU_CNTL_REG_RX_MASK		(~(1 << 0))

#define AUX_MU_CNTL_REG_DISABLE_ALL	(0x0)

#define UART0_DR_REG_ADDR		(MMIO_BASE + 0x00201000) 
#define UART0_FR_REG_ADDR		(MMIO_BASE + 0x00201018)
#define UART0_IBRD_REG_ADDR		(MMIO_BASE + 0x00201024)
#define UART0_FBRD_REG_ADDR		(MMIO_BASE + 0x00201028)
#define UART0_LCRH_REG_ADDR		(MMIO_BASE + 0x0020102C)
#define UART0_CR_REG_ADDR		(MMIO_BASE + 0x00201030)
#define UART0_ICR_REG_ADDR		(MMIO_BASE + 0x00201044)

#define UART0_FR_REG_TX_FULL		(1 << 5)
#define UART0_FR_REG_RX_EMPTY		(1 << 4)

#define UART0_LCRH_REG_WLEN_8_BITS  (0x3 << 5)
#define UART0_LCRH_REG_WLEN_7_BITS  (0x2 << 5)
#define UART0_LCRH_REG_WLEN_6_BITS  (0x1 << 5)
#define UART0_LCRH_REG_WLEN_5_BITS  (0)
#define UART0_LCRH_REG_WLEN_MASK 	(~(0x3 << 5))

#define UART0_LCRH_REG_FEN_ENABLE 	(0x1 << 4)
#define UART0_LCRH_REG_FEN_DISABLE 	(0)
#define UART0_LCRH_REG_FEN_MASK 	(~(0x1 << 4))


#define UART0_CR_REG_DISABLE_ALL 	(0)
#define UART0_CR_REG_RX_ENABLE		(1 << 9)
#define UART0_CR_REG_TX_ENABLE		(1 << 8)
#define UART0_CR_REG_UART_ENABLE	(1 << 0)

#define UART0_ICR_REG_CLEAR_ALL_ITR	0x7FF

#ifndef _ASM_

enum UART_TYPE
{
	type_mini_uart,
	type_uart0
};

bool uart_init(enum UART_TYPE type);
void uart_send(unsigned int c);
char uart_getc();
void uart_puts(const char *s);
void uart_flush();

#endif

#endif