#include "uart.h"
#include "gpio.h"
#include "mailbox.h"
#include "util.h"

typedef void (*init_op)();
typedef void (*send_op)(char);
typedef char (*getc_op)();
typedef void (*flush_op)();

typedef struct uart_ops 
{	
	init_op 	init;
	send_op 	send;
	getc_op		getc;
	flush_op	flush;
}UART_OPS,*PUART_OPS;

typedef struct uart
{
	enum UART_TYPE type;
	UART_OPS ops;
}UART,*PUART;

static void mini_uart_init();
static void mini_uart_send(char c);
static char mini_uart_getc();
static void mini_uart_flush();

static void uart0_init();
static void uart0_send(char c);
static char uart0_getc();
static void uart0_flush();

static UART g_uart[] = 
{
	{
		.type = type_mini_uart,
		.ops = 
		{
			.init = mini_uart_init,
			.send = mini_uart_send,
			.getc = mini_uart_getc,
			.flush = mini_uart_flush,
		},		
	},
	{
		.type = type_uart0,
		.ops = 
		{
			.init = uart0_init,
			.send = uart0_send,
			.getc = uart0_getc,
			.flush = uart0_flush,
		},			
	},
};

static int g_uart_size = sizeof(g_uart) / sizeof(UART);

static PUART g_cur_uart = NULL;

static void mini_uart_init()
{
	register unsigned int r;
	unsigned int val = 0;

	//Enable Uart
	val = get(AUX_ENABLES_ADDR);
	val |= AUX_ENABLES_UART_ENABLE;
	set(AUX_ENABLES_ADDR,val);

	//Disbale CTS RTS RX TX
	set(AUX_MU_CNTL_REG_ADDR,AUX_MU_CNTL_REG_DISABLE_ALL);

	//Set 8 bits mode
	val = get(AUX_MU_LCR_REG_ADDR);
	val |= AUX_MU_LCR_REG_8BIT_MODE;
	set(AUX_MU_LCR_REG_ADDR,AUX_MU_LCR_REG_8BIT_MODE);

	//Disable all interrupts
	set(AUX_MU_IER_REG_ADDR, 0);
	set(AUX_MU_IIR_REG_ADDR, 0xc6);
	
	set(AUX_MU_MCR_REG_ADDR,0);

	//Set baudrate to 115200
	set(AUX_MU_BAUD_REG_ADDR,270); 

	//Set GPIO14 to TXD1
	//Set GPIO15 to RXD1
	val = get(GPIO_GPFSEL1_ADDR);
	val = (val & GPIO_FUNCTION_SELECT_MASK(14)) | GPIO_FUNCTION_SELECT_FUNC_5(14);
	val = (val & GPIO_FUNCTION_SELECT_MASK(15)) | GPIO_FUNCTION_SELECT_FUNC_5(15);
	set(GPIO_GPFSEL1_ADDR,val);

	set(GPIO_GPPUD_ADDR,0);
	r=150; while(r--){asm volatile("nop");}
	val = GPIO_GPPUDCLK0_ASSERT_LINE(14) | GPIO_GPPUDCLK0_ASSERT_LINE(15);
	set(GPIO_GPPUDCLK0_ADDR,val);
	r=150; while(r--){asm volatile("nop");}
	set(GPIO_GPPUDCLK0_ADDR,0);

	val = AUX_MU_CNTL_REG_TX_ENABLE | AUX_MU_CNTL_REG_RX_ENABLE;
	set(AUX_MU_CNTL_REG_ADDR,val);	
}

static void mini_uart_send(char c)
{
	unsigned int val;
	do
	{
		val = get(AUX_MU_LSR_REG_ADDR);
		asm volatile("nop");	
	}while(!(val & AUX_MU_LSR_REG_TX_EMPTY));

	set(AUX_MU_IO_REG_ADDR,c);
}

static char mini_uart_getc()
{
	char c;
	unsigned int val;
	do
	{
		val = get(AUX_MU_LSR_REG_ADDR);
		asm volatile("nop");
	}while(!(val & AUX_MU_LSR_REG_RX_READY));

	c = (char)get(AUX_MU_IO_REG_ADDR);

	return c == '\r' ? '\n' : c;

}

static void mini_uart_flush()
{
	while (get(AUX_MU_LSR_REG_ADDR) & 0x01) {
        get(AUX_MU_IO_REG_ADDR);
    }
}

static void uart0_init()
{
	register unsigned int r;
	unsigned int val = 0;

	//Disbale UART
	val = UART0_CR_REG_DISABLE_ALL;
	set(UART0_CR_REG_ADDR,val);

	//mailbox_set_clock_rate(MAILBOX_UART_CLOCK_ID,4000000);

	//Set GPIO14 to TXD0
	//Set GPIO15 to RXD0
	val = get(GPIO_GPFSEL1_ADDR);
	val = (val & GPIO_FUNCTION_SELECT_MASK(14)) | GPIO_FUNCTION_SELECT_FUNC_0(14);
	val = (val & GPIO_FUNCTION_SELECT_MASK(15)) | GPIO_FUNCTION_SELECT_FUNC_0(15);
	set(GPIO_GPFSEL1_ADDR,val);

	set(GPIO_GPPUD_ADDR,0);
	r=150; while(r--){asm volatile("nop");}
	val = GPIO_GPPUDCLK0_ASSERT_LINE(14) | GPIO_GPPUDCLK0_ASSERT_LINE(15);
	set(GPIO_GPPUDCLK0_ADDR,val);
	r=150; while(r--){asm volatile("nop");}
	set(GPIO_GPPUDCLK0_ADDR,0);

	//Disable all interrupts
	set(UART0_ICR_REG_ADDR,UART0_ICR_REG_CLEAR_ALL_ITR);

	//Set baudrate to 115200
	//set(UART0_IBRD_REG_ADDR,2);
	//set(UART0_FBRD_REG_ADDR,0xB);

	set(UART0_IBRD_REG_ADDR,26);
	set(UART0_FBRD_REG_ADDR,3);

	//Set 8 bits mode
	//Diable FIFO
	val = get(UART0_LCRH_REG_ADDR);
	val &= UART0_LCRH_REG_WLEN_MASK;
	val |= UART0_LCRH_REG_WLEN_8_BITS;
	val &= UART0_LCRH_REG_FEN_MASK;
	val |= UART0_LCRH_REG_FEN_DISABLE;
	set(UART0_LCRH_REG_ADDR,val);

	val = get(UART0_CR_REG_ADDR);
	val = UART0_CR_REG_TX_ENABLE | UART0_CR_REG_RX_ENABLE | UART0_CR_REG_UART_ENABLE;
	set(UART0_CR_REG_ADDR,val);
}

static void uart0_send(char c)
{
	unsigned int val;
	do
	{
		val = get(UART0_FR_REG_ADDR);
		asm volatile("nop");	
	}while((val & UART0_FR_REG_TX_FULL));

	set(UART0_DR_REG_ADDR,c);	
}

static char uart0_getc()
{
	char c;
	unsigned int val;
	do
	{
		val = get(UART0_FR_REG_ADDR);
		asm volatile("nop");
	}while((val & UART0_FR_REG_RX_EMPTY));

	c = (char)get(UART0_DR_REG_ADDR);

	return c == '\r' ? '\n' : c;	
}

static void uart0_flush()
{
	while (!(get(UART0_FR_REG_ADDR) & UART0_FR_REG_RX_EMPTY)) {
        get(UART0_DR_REG_ADDR);
    }
}

bool uart_init(enum UART_TYPE type)
{
	g_cur_uart = NULL;
	for(int i = 0; i < g_uart_size; i++)
	{
		if(g_uart[i].type == type)
		{
			g_cur_uart = &g_uart[i];
			break;
		}
	}

	if(g_cur_uart == NULL)
		return false;

	g_cur_uart->ops.init();

	return true;
}	

void uart_send(unsigned int c)
{
	if(g_cur_uart)
		g_cur_uart->ops.send(c);
}

char uart_getc()
{
	if(g_cur_uart)
		return g_cur_uart->ops.getc();			

	return 0;

}

void uart_puts(const char *s)
{
	while(*s)
	{
		if(*s == '\n')
			uart_send('\r');
		
		uart_send(*s++);
	}			
}

void uart_flush()
{
	if(g_cur_uart)
		return g_cur_uart->ops.flush();
}
