#ifndef _SYSREGS_H

#define HCR_EL2_RW_AARCH64	(1 << 31)

#define SCTLR_ELx_M	(1<<0)
#define SCTLR_ELx_C	(1<<2) /*data cache enable*/

#define SCTLR_EL1_EE_MASK 	(~(1 << 25))
#define SCTLR_EL1_EE_LITTLE (0 << 25)

#define SCTLR_EL1_EOE_MASK		(~(1 << 24))
#define SCTLR_EL1_EOE_LITTLE	(0 << 24)
#define SCTLR_EL1_M_MASK		(~(1 << 0))
#define SCTLR_EL1_M_DISABLE_MMU	(0 << 0)



#define SPSR_EL2_DAIF_DISABLE	(7 << 6)

#define SPSR_EL2_M_MASK	(~(0b11111))
#define SPSR_EL2_M_EL1H	(0b0101)

#define CPACR_EL1_FPEN      (0b11 << 20)

#define read_sysreg(reg) ({ \
		unsigned long _val; \
		asm volatile("mrs %0," #reg \
		: "=r"(_val)); \
		_val; \
})

#define write_sysreg(val, reg) ({ \
		unsigned long _val = (unsigned long)val; \
		asm volatile("msr " #reg ", %x0" \
		:: "rZ"(_val)); \
})

#endif