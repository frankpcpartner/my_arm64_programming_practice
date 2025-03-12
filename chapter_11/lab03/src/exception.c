#include "exception.h"
#include "util.h"
#include "sysregs.h"

static const char* const bad_mode_handler[] = 
{
	"Sync Abort",
	"IRQ",
	"FIRQ",
	"SError",
};

read_sysreg(far_el1)

void bad_mod(long long sp_address,int reason, unsigned long long esr)
{
	printf("\rBad mode for %s handler detected, far:0x%x esr:%0x%x\n",
		bad_mode_handler[reason], read_sysreg_far_el1(), esr);
}