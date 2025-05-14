#include "cache.h"
#include "sysregs.h"
#include "util.h"

static const char * cache_type_string[] = {
	"nocache",
	"i-cache",
	"d-cache",
	"separate cache",
	"unifed cache"
};

static const char *icache_policy_str[] = {
	[0 ... ICACHE_POLICY_PIPT]	= "RESERVED/UNKNOWN",
	[ICACHE_POLICY_VIPT]		= "VIPT",
	[ICACHE_POLICY_PIPT]		= "PIPT",
	[ICACHE_POLICY_VPIPT]		= "VPIPT",
};

static inline int cache_line_size(unsigned long long ctr_el0)
{
	unsigned int cwg = CTR_EL0_CWG(ctr_el0);
	return 4 << cwg;
}

static void show_cache_set_way(
	int level,
	int ind) 
{
	unsigned long long val = 0;
	unsigned long long tmp = (level -1) << CSSELR_EL1_LEVEL_SHIFT | ind;
	unsigned int line_size, set, way;

	write_sysreg(tmp, CSSELR_EL1);
    
    val = read_sysreg(CCSIDR_EL1);

    set = (val >> CCSIDR_EL1_NUMSETS_SHIFT) & CCSIDR_EL1_NUMSETS_MASK;
	set += 1;
	way = (val >> CCSIDR_EL1_ASS_SHIFT) & CCSIDR_EL1_ASS_MASK;
	way += 1;

	line_size = (val & CCSIDR_EL1_LINESIZE_MASK);
	line_size = 1 << (line_size + 4);

    printf("\r 	%s: set %u way %u line_size %u size %uKB\n",
			ind ? "i-cache":"d/u cache", set, way, line_size,
			(line_size * way * set)/1024);
}


static bool show_cache_type(
	unsigned long long clidr_el1, 
	unsigned long long ctrl_el0,
	int level)
{
	int ctype = CLIDR_EL1_CTYPE(clidr_el1,level);

	if(ctype > CLIDR_EL1_CTYPE_UC || ctype == CLIDR_EL1_CTYPE_NA)
		return false;

	printf("\r L%u: %s, cache line size %u\n",
		level, cache_type_string[ctype], cache_line_size(ctrl_el0));

	if(ctype == CLIDR_EL1_CTYPE_SC) 
	{
		show_cache_set_way(level, 1);
		show_cache_set_way(level, 0);
	}
	else
	{
		show_cache_set_way(level, 0);	
	}

	return true;
}


void show_cache_info()
{
	unsigned long long clidr_el1 = read_sysreg(CLIDR_EL1);
	unsigned long long ctr_el0 = read_sysreg(CTR_EL0);

	unsigned long long l1ip = CTR_EL0_L1IP(ctr_el0);

	printf("\r IBC:%u LOUU:%u LoC:%u LoUIS:%u\n",
			CLIDR_EL1_ICB(clidr_el1), CLIDR_EL1_LOUU(clidr_el1),
			CLIDR_EL1_LOC(clidr_el1), CLIDR_EL1_LOUIS(clidr_el1));

	for(int i = 1; i < CLIDR_EL1_CTYPE_MAX; i++)
	{
		if(!show_cache_type(clidr_el1, ctr_el0, i))
			break;
	}

	printf("\r Detected %s I-cache\n", icache_policy_str[l1ip]);

}	
