#ifndef _EXCEPTION_H
#define _EXCEPTION_H

#include "core.h"

#ifndef _BUILD_ASM_OFFSET_
#include "asm-offsets.h"
#endif

#define BAD_SYNC        0
#define BAD_IRQ         1
#define BAD_FIQ         2
#define BAD_ERROR       3 

#define ESR_ELx_EC_UNKNOWN	(0x00)
#define ESR_ELx_EC_WFx		(0x01)
/* Unallocated EC: 0x02 */
#define ESR_ELx_EC_CP15_32	(0x03)
#define ESR_ELx_EC_CP15_64	(0x04)
#define ESR_ELx_EC_CP14_MR	(0x05)
#define ESR_ELx_EC_CP14_LS	(0x06)
#define ESR_ELx_EC_FP_ASIMD	(0x07)
#define ESR_ELx_EC_CP10_ID	(0x08)	/* EL2 only */
#define ESR_ELx_EC_PAC		(0x09)	/* EL2 and above */
/* Unallocated EC: 0x0A - 0x0B */
#define ESR_ELx_EC_CP14_64	(0x0C)
/* Unallocated EC: 0x0d */
#define ESR_ELx_EC_ILL		(0x0E)
/* Unallocated EC: 0x0F - 0x10 */
#define ESR_ELx_EC_SVC32	(0x11)
#define ESR_ELx_EC_HVC32	(0x12)	/* EL2 only */
#define ESR_ELx_EC_SMC32	(0x13)	/* EL2 and above */
/* Unallocated EC: 0x14 */
#define ESR_ELx_EC_SVC64	(0x15)
#define ESR_ELx_EC_HVC64	(0x16)	/* EL2 and above */
#define ESR_ELx_EC_SMC64	(0x17)	/* EL2 and above */
#define ESR_ELx_EC_SYS64	(0x18)
#define ESR_ELx_EC_SVE		(0x19)
/* Unallocated EC: 0x1A - 0x1E */
#define ESR_ELx_EC_IMP_DEF	(0x1f)	/* EL3 only */
#define ESR_ELx_EC_IABT_LOW	(0x20)
#define ESR_ELx_EC_IABT_CUR	(0x21)
#define ESR_ELx_EC_PC_ALIGN	(0x22)
/* Unallocated EC: 0x23 */
#define ESR_ELx_EC_DABT_LOW	(0x24)
#define ESR_ELx_EC_DABT_CUR	(0x25)
#define ESR_ELx_EC_SP_ALIGN	(0x26)
/* Unallocated EC: 0x27 */
#define ESR_ELx_EC_FP_EXC32	(0x28)
/* Unallocated EC: 0x29 - 0x2B */
#define ESR_ELx_EC_FP_EXC64	(0x2C)
/* Unallocated EC: 0x2D - 0x2E */
#define ESR_ELx_EC_SERROR	(0x2F)
#define ESR_ELx_EC_BREAKPT_LOW	(0x30)
#define ESR_ELx_EC_BREAKPT_CUR	(0x31)
#define ESR_ELx_EC_SOFTSTP_LOW	(0x32)
#define ESR_ELx_EC_SOFTSTP_CUR	(0x33)
#define ESR_ELx_EC_WATCHPT_LOW	(0x34)
#define ESR_ELx_EC_WATCHPT_CUR	(0x35)
/* Unallocated EC: 0x36 - 0x37 */
#define ESR_ELx_EC_BKPT32	(0x38)
/* Unallocated EC: 0x39 */
#define ESR_ELx_EC_VECTOR32	(0x3A)	/* EL2 only */
/* Unallocted EC: 0x3B */
#define ESR_ELx_EC_BRK64	(0x3C)
/* Unallocated EC: 0x3D - 0x3F */
#define ESR_ELx_EC_MAX		(0x3F)

#define ESR_ELx_EC_SHIFT	(26)
#define ESR_ELx_EC_MASK		((unsigned int)(0x3F) << ESR_ELx_EC_SHIFT)
#define ESR_ELx_EC(esr)		(((esr) & ESR_ELx_EC_MASK) >> ESR_ELx_EC_SHIFT)

#define ESR_ELx_IL_16BITS	(0x0)
#define ESR_ELx_IL_32BITS	(0x1)

#define ESR_ELx_IL_SHIFT	(25)
#define ESR_ELx_IL		((unsigned int)(1) << ESR_ELx_IL_SHIFT)

/* Shared ISS field definitions for Data/Instruction aborts */
#define ESR_ELx_SET_SHIFT	(11)
#define ESR_ELx_SET_MASK	((unsigned int)(3) << ESR_ELx_SET_SHIFT)
#define ESR_ELx_FnV_SHIFT	(10)
#define ESR_ELx_FnV		((unsigned int)(1) << ESR_ELx_FnV_SHIFT)
#define ESR_ELx_EA_SHIFT	(9)
#define ESR_ELx_EA		((unsigned int)(1) << ESR_ELx_EA_SHIFT)
#define ESR_ELx_S1PTW_SHIFT	(7)
#define ESR_ELx_S1PTW		((unsigned int)(1) << ESR_ELx_S1PTW_SHIFT)

#define ESR_ELx_DFSC_ADDR_FAULT_LV0		(0b000000)
#define ESR_ELx_DFSC_ADDR_FAULT_LV1		(0b000001)
#define ESR_ELx_DFSC_ADDR_FAULT_LV2		(0b000010)
#define ESR_ELx_DFSC_ADDR_FAULT_LV3		(0b000011)
#define ESR_ELx_DFSC_TRAN_FAULT_LV0		(0b000100)
#define ESR_ELx_DFSC_TRAN_FAULT_LV1		(0b000101)
#define ESR_ELx_DFSC_TRAN_FAULT_LV2		(0b000110)
#define ESR_ELx_DFSC_TRAN_FAULT_LV3		(0b000111)
#define ESR_ELx_DFSC_ACCESS_FAULT_LV1	(0b001001)
#define ESR_ELx_DFSC_ACCESS_FAULT_LV2	(0b001010)
#define ESR_ELx_DFSC_ACCESS_FAULT_LV3	(0b001011)
#define ESR_ELx_DFSC_PERM_FAULT_LV1		(0b001101)
#define ESR_ELx_DFSC_PERM_FAULT_LV2		(0b001110)
#define ESR_ELx_DFSC_PERM_FAULT_LV3		(0b001111)
#define ESR_ELx_DFSC_EXTABT				(0b010000)
#define ESR_ELx_DFSC_SERROR				(0b010001)
#define ESR_ELx_DFSC_EXTABT_LV0			(0b010100)
#define ESR_ELx_DFSC_EXTABT_LV1			(0b010101)
#define ESR_ELx_DFSC_EXTABT_LV2			(0b010110)
#define ESR_ELx_DFSC_EXTABT_LV3			(0b010111)
#define ESR_ELx_DFSC_PARITY_ERR_MM		(0b011000)
#define ESR_ELx_DFSC_PARITY_ERR_LV0		(0b011011)
#define ESR_ELx_DFSC_PARITY_ERR_LV1		(0b011100)
#define ESR_ELx_DFSC_PARITY_ERR_LV2		(0b011101)
#define ESR_ELx_DFSC_PARITY_ERR_LV3		(0b011110)
#define ESR_ELx_DFSC_ALIGN_FAULT 		(0b100001)
#define ESR_ELx_DFSC_TLB_CONFLICT		(0b110000)
#define ESR_ELx_DFSC_ATOMIC_HW_FAULT	(0b110001)
#define ESR_ELx_DFSC_SEG_FAULT			(0b111101)
#define ESR_ELx_DFSC_PAGE_FAULT			(0b111110)
#define ESR_ELx_DFSC_MAX				0x3f

#define ESR_ELx_DFSC(esr)	(((esr) & 0x3f))

#define ARM_LOCAL_IRQ_SOURCE0_REG			0x40000060
#define ARM_LOCAL_IRQ_SOURCE0_CNTPNS(reg)	(((reg) & 0b10))

#ifndef _ASM_

typedef struct pt_regs_s 
{
    struct {
        unsigned long long regs[31];
        unsigned long long sp;
        unsigned long long pc;
        unsigned long long pstate;
    };

    unsigned long long orig_x0;
    unsigned int syscallno;
    unsigned int unused2;

    unsigned long long orig_addr_limit;
    unsigned long long unused;
    unsigned long long stackframe[2];
}pt_regs_t;

#endif

#endif