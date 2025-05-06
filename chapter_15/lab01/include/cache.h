#ifndef _CACHE_H
#define _CACHE_H

#define CLIDR_EL1_CTYPE_MAX	7

#define CLIDR_EL1_CTYPE(reg,n)	(((unsigned long long)reg & ((7ULL) << ((n - 1) * 3))) >> ((n - 1) * 3))   

#define CLIDR_EL1_CTYPE_NA (0b000)
#define CLIDR_EL1_CTYPE_IC (0b001)
#define CLIDR_EL1_CTYPE_DC (0b010)
#define CLIDR_EL1_CTYPE_SC (0b011)
#define CLIDR_EL1_CTYPE_UC (0b100)

#define CLIDR_EL1_LOUIS(reg)	(((unsigned long long)reg >> 21) & 7ULL)
#define CLIDR_EL1_LOC(reg)		(((unsigned long long)reg >> 24) & 7ULL)
#define CLIDR_EL1_LOUU(reg)		(((unsigned long long)reg >> 27) & 7ULL)
#define CLIDR_EL1_ICB(reg)		(((unsigned long long)reg >> 30) & 7ULL)


#define ICACHE_POLICY_VPIPT	0
#define ICACHE_POLICY_VIPT	2
#define ICACHE_POLICY_PIPT	3

#define CTR_EL0_L1IP(reg)		(((unsigned long long)reg >> 14) & 3ULL)
#define CTR_EL0_CWG(reg)		(((unsigned long long)reg >> 24) & 0xfULL)


#define CSSELR_EL1_InD				(0b1)
#define CSSELR_EL1_LEVEL_SHIFT 		1
#define CCSIDR_EL1_NUMSETS_SHIFT 	13
#define CCSIDR_EL1_NUMSETS_MASK		(0x1fff)
#define CCSIDR_EL1_LINESIZE_SHIFT 	0
#define CCSIDR_EL1_LINESIZE_MASK 	(0x7)
#define CCSIDR_EL1_ASS_SHIFT 		3
#define CCSIDR_EL1_ASS_MASK 		(0x3ff)



void show_cache_info();

#endif