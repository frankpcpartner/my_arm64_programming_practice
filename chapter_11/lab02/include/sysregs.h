#ifndef _SYSREGS_H

#define HCR_EL2_RW_AARCH64	(1 << 31)

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

#define read_sysreg(reg)					\
static inline long long read_sysreg_##reg()	\
{											\
	long long ret = 0;						\
											\
	asm volatile(							\
		" mrs %[ret], "#reg" \n"			\
		:[ret]"=r"(ret)						\
		:									\
		:"memory"							\
	);										\
	return ret;								\
}	

#endif
