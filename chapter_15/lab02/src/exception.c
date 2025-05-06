#include "exception.h"
#include "util.h"
#include "sysregs.h"
#include "timer.h"

static const char* const bad_mode_handler[] = 
{
	"Sync Abort",
	"IRQ",
	"FIRQ",
	"SError",
};

static const char *esr_class_str[] = {
	[0 ... ESR_ELx_EC_MAX]		= "UNRECOGNIZED EC",
	[ESR_ELx_EC_UNKNOWN]		= "Unknown/Uncategorized",
	[ESR_ELx_EC_WFx]		= "WFI/WFE",
	[ESR_ELx_EC_CP15_32]		= "CP15 MCR/MRC",
	[ESR_ELx_EC_CP15_64]		= "CP15 MCRR/MRRC",
	[ESR_ELx_EC_CP14_MR]		= "CP14 MCR/MRC",
	[ESR_ELx_EC_CP14_LS]		= "CP14 LDC/STC",
	[ESR_ELx_EC_FP_ASIMD]		= "ASIMD",
	[ESR_ELx_EC_CP10_ID]		= "CP10 MRC/VMRS",
	[ESR_ELx_EC_CP14_64]		= "CP14 MCRR/MRRC",
	[ESR_ELx_EC_ILL]		= "PSTATE.IL",
	[ESR_ELx_EC_SVC32]		= "SVC (AArch32)",
	[ESR_ELx_EC_HVC32]		= "HVC (AArch32)",
	[ESR_ELx_EC_SMC32]		= "SMC (AArch32)",
	[ESR_ELx_EC_SVC64]		= "SVC (AArch64)",
	[ESR_ELx_EC_HVC64]		= "HVC (AArch64)",
	[ESR_ELx_EC_SMC64]		= "SMC (AArch64)",
	[ESR_ELx_EC_SYS64]		= "MSR/MRS (AArch64)",
	[ESR_ELx_EC_SVE]		= "SVE",
	[ESR_ELx_EC_IMP_DEF]		= "EL3 IMP DEF",
	[ESR_ELx_EC_IABT_LOW]		= "IABT (lower EL)",
	[ESR_ELx_EC_IABT_CUR]		= "IABT (current EL)",
	[ESR_ELx_EC_PC_ALIGN]		= "PC Alignment",
	[ESR_ELx_EC_DABT_LOW]		= "DABT (lower EL)",
	[ESR_ELx_EC_DABT_CUR]		= "DABT (current EL)",
	[ESR_ELx_EC_SP_ALIGN]		= "SP Alignment",
	[ESR_ELx_EC_FP_EXC32]		= "FP (AArch32)",
	[ESR_ELx_EC_FP_EXC64]		= "FP (AArch64)",
	[ESR_ELx_EC_SERROR]		= "SError",
	[ESR_ELx_EC_BREAKPT_LOW]	= "Breakpoint (lower EL)",
	[ESR_ELx_EC_BREAKPT_CUR]	= "Breakpoint (current EL)",
	[ESR_ELx_EC_SOFTSTP_LOW]	= "Software Step (lower EL)",
	[ESR_ELx_EC_SOFTSTP_CUR]	= "Software Step (current EL)",
	[ESR_ELx_EC_WATCHPT_LOW]	= "Watchpoint (lower EL)",
	[ESR_ELx_EC_WATCHPT_CUR]	= "Watchpoint (current EL)",
	[ESR_ELx_EC_BKPT32]		= "BKPT (AArch32)",
	[ESR_ELx_EC_VECTOR32]		= "Vector catch (AArch32)",
	[ESR_ELx_EC_BRK64]		= "BRK (AArch64)",
};

static const char *esr_dfsc_str[] = {
	[0 ... ESR_ELx_DFSC_MAX]		= "UNRECOGNIZED DFSC",
	[ESR_ELx_DFSC_ADDR_FAULT_LV0] = "Address size fault, level 0.",
	[ESR_ELx_DFSC_ADDR_FAULT_LV1] = "Address size fault, level 1.",
	[ESR_ELx_DFSC_ADDR_FAULT_LV2] = "Address size fault, level 2.",
	[ESR_ELx_DFSC_ADDR_FAULT_LV3] = "Address size fault, level 3.",
	[ESR_ELx_DFSC_TRAN_FAULT_LV0] = "Translation fault, level 0.",
	[ESR_ELx_DFSC_TRAN_FAULT_LV1] = "Translation fault, level 1.",
	[ESR_ELx_DFSC_TRAN_FAULT_LV2] = "Translation fault, level 2.",
	[ESR_ELx_DFSC_TRAN_FAULT_LV3] = "Translation fault, level 3.",
	[ESR_ELx_DFSC_ACCESS_FAULT_LV1] = "Access flag fault, level 1.",
	[ESR_ELx_DFSC_ACCESS_FAULT_LV2] = "Access flag fault, level 2.",
	[ESR_ELx_DFSC_ACCESS_FAULT_LV3] = "Access flag fault, level 3.",
	[ESR_ELx_DFSC_PERM_FAULT_LV1] = "Permission fault, level 1.",
	[ESR_ELx_DFSC_PERM_FAULT_LV2] = "Permission fault, level 2.",
	[ESR_ELx_DFSC_PERM_FAULT_LV3] = "Permission fault, level 3.",
	[ESR_ELx_DFSC_EXTABT		] = "Synchronous External abort.",
	[ESR_ELx_DFSC_SERROR		] = "Synchronous Tag Check Fault.",
	[ESR_ELx_DFSC_EXTABT_LV0	] = "Synchronous External abort, level 0.",
	[ESR_ELx_DFSC_EXTABT_LV1	] = "Synchronous External abort, level 1.",
	[ESR_ELx_DFSC_EXTABT_LV2	] = "Synchronous External abort, level 2.",
	[ESR_ELx_DFSC_EXTABT_LV3	] = "Synchronous External abort, level 3.",
	[ESR_ELx_DFSC_PARITY_ERR_MM] = "Synchronous parity or ECC error on memory access.",
	[ESR_ELx_DFSC_PARITY_ERR_LV0] = "Synchronous parity or ECC error on memory access, level 0.",
	[ESR_ELx_DFSC_PARITY_ERR_LV1] = "Synchronous parity or ECC error on memory access, level 1.",
	[ESR_ELx_DFSC_PARITY_ERR_LV2] = "Synchronous parity or ECC error on memory access, level 2.",
	[ESR_ELx_DFSC_PARITY_ERR_LV3] = "Synchronous parity or ECC error on memory access, level 3.",
	[ESR_ELx_DFSC_ALIGN_FAULT ] = "Alignment fault.",
	[ESR_ELx_DFSC_TLB_CONFLICT] = "TLB conflict abort.",
	[ESR_ELx_DFSC_ATOMIC_HW_FAULT] = "Unsupported atomic hardware update fault.",
	[ESR_ELx_DFSC_SEG_FAULT] = "Segmentation Fault.",
	[ESR_ELx_DFSC_PAGE_FAULT] = "Page Fault.",
};


static const char *esr_get_class_string(unsigned int esr)
{
	return esr_class_str[ESR_ELx_EC(esr)];
}

static const char *esr_get_dfsc_string(unsigned int esr)
{
	return esr_dfsc_str[ESR_ELx_DFSC(esr)];
}

void bad_mode(long long sp_address,int reason, unsigned int esr, unsigned long long elr)
{
	printf("\rBad mode for %s handler detected, far:0x%x esr:0x%x elr:0x%llx\n",
		bad_mode_handler[reason], read_sysreg(far_el1), esr, elr);

	unsigned long long class = ESR_ELx_EC(esr);

	if(class == ESR_ELx_EC_DABT_LOW || class == ESR_ELx_EC_DABT_CUR)
	{
		printf("\rESR info:\n");
		printf("\r ESR = 0x%x\n",esr);
		printf("\r Exception class = %s, IL = %u bits\n",
			esr_get_class_string(esr),(esr & ESR_ELx_IL) ? 32 : 16);
		printf("\r Data abort:\n");
		printf("\r SET = %llu, FnV = %llu\n",
			(esr & ESR_ELx_SET_MASK) >> ESR_ELx_SET_SHIFT,
			(esr & ESR_ELx_FnV) >> ESR_ELx_FnV_SHIFT);
		printf("\r EA = %llu, S1PTW = %llu\n",
			(esr & ESR_ELx_EA) >> ESR_ELx_EA_SHIFT,
			(esr & ESR_ELx_S1PTW) >> ESR_ELx_S1PTW_SHIFT);
		printf("\r DFSC = %s\n",esr_get_dfsc_string(esr));
	}
}

void irq_handle()
{
	unsigned int irq = get(ARM_LOCAL_IRQ_SOURCE0_REG);
	
	if(ARM_LOCAL_IRQ_SOURCE0_CNTPNS(irq))
	{
		handle_timer_irq();
	}
	else
	{	
		printf("\rUnknown peding irq: 0x%x\n",irq);
	}
}