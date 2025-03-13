#define _ASM_

#include "uart.h"
#include "gpio.h"

.section .text
.global _early_uart_init
.global _printel

.align 3
_early_uart_init:
	stp x29, x30, [sp,#-16] !
	
	//Disbale UART
	ldr x0,=UART0_CR_REG_ADDR
	ldr x1,=UART0_CR_REG_DISABLE_ALL
	str w1,[x0]

	//Set GPIO14 to TXD0
	//Set GPIO15 to RXD0
	ldr x0, =GPIO_GPFSEL1_ADDR
	ldr w1, [x0]
	
	ldr x2, =GPIO_FUNCTION_SELECT_MASK(14)
	ldr x3, =GPIO_FUNCTION_SELECT_FUNC_0(14)
	and w1, w1, w2 
	orr w1, w1, w3

	ldr x2, =GPIO_FUNCTION_SELECT_MASK(15)
	ldr x3, =GPIO_FUNCTION_SELECT_FUNC_0(15)
	and w1, w1, w2
	orr w1, w1, w3
	str w1, [x0] 

	ldr x0, =GPIO_GPPUD_ADDR
	str wzr, [x0]
	mov x1, 150
1:	nop
	sub x1, x1, 1
	cbnz x1, 1b

	ldr x0,=GPIO_GPPUDCLK0_ADDR
	ldr x1,=GPIO_GPPUDCLK0_ASSERT_LINE(14)
	orr w1, w1, GPIO_GPPUDCLK0_ASSERT_LINE(15)
	str w1, [x0]
	mov x1, 150
1:	nop
	sub x1, x1, 1
	cbnz x1, 1b
	str w1, [x0]
	
	isb

	//Disable all interrupts
	ldr x0,=UART0_ICR_REG_ADDR
	ldr x1,=UART0_ICR_REG_CLEAR_ALL_ITR
	str w1, [x0]

	//Set baudrate to 115200
	ldr x0, =UART0_IBRD_REG_ADDR
	mov x1, #26
	str w1, [x0]

	ldr x0, =UART0_FBRD_REG_ADDR
	mov x1, #3
	str w1, [x0]
	
	//Set 8 bits mode
	//Diable FIFO
	ldr x0, =UART0_LCRH_REG_ADDR
	ldr w1, [x0]

	ldr x2, =UART0_LCRH_REG_WLEN_MASK
	ldr x3, =UART0_LCRH_REG_WLEN_8_BITS
	ldr x4, =UART0_LCRH_REG_FEN_MASK
	ldr x5, =UART0_LCRH_REG_FEN_DISABLE
	
	and w1, w1, w2
	orr w1, w1, w3
	and w1, w1, w4
	orr w1, w1, w5
	str w1, [x0]

	ldr x0, =UART0_CR_REG_ADDR
	ldr x2, =(UART0_CR_REG_TX_ENABLE | UART0_CR_REG_RX_ENABLE | UART0_CR_REG_UART_ENABLE)
	ldr w1, [x0]
	orr w1, w1, w2
	str w1, [x0]

	ldp x29, x30, [sp], #16
	ret

_printel:
	stp x29, x30, [sp,#-16] !
	add sp, sp, #-16

	adrp x1, str_printel_fmt
	add x1, x1, :lo12:str_printel_fmt

1:	ldrb w0, [x1], #1   
	cbz w0, 2f 
	
	stp x0, x1, [sp]
	bl _uart_put
	ldp x0, x1, [sp]
	
	b 1b

2:	mrs x0, CurrentEL 
	lsr x0, x0, #2
	add x0, x0, #0x30 //ascii code for '0'

	bl _uart_put

	mov x0, #0xa //ascii code for '\n'
	bl _uart_put

	add sp, sp, #16
	ldp x29, x30, [sp], #16
	ret


//w0: the word to be printed	
_uart_put:
	stp x29, x30, [sp,#-16] !
	
	ldr x1, =UART0_FR_REG_ADDR
	ldr x2, =UART0_DR_REG_ADDR

1:	ldr w3, [x1]
	and w3, w3, UART0_FR_REG_TX_FULL
	nop
	cbnz w3, 1b

	str w0, [x2]
	
	ldp x29, x30, [sp], #16
	ret

.section .rodata
.align 3
str_printel_fmt:
	.string "\rBooting at EL"
