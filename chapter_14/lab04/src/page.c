#include "page.h"
#include "util.h"
#include "sysregs.h"

extern char _text_boot_start[];
extern char _text_end[];
extern char idmap_pg_dir[]; 
extern char _data_end[];

static unsigned short mem_map[NR_PAGES] = {0};


static unsigned long long get_free_page()
{
	int i;

	for(i = 0; i < NR_PAGES; i++){
		if(mem_map[i] == 0){
			mem_map[i] = 1;
			return LOW_MEMORY + i * PAGE_SIZE;
		}
	}

	return 0;
}

void free_page(unsigned long long p)
{
	mem_map[(p - LOW_MEMORY) / PAGE_SIZE] = 0;
}


static unsigned long long early_pgtable_alloc()
{
	unsigned long long phys;
	phys = get_free_page();
	memset((void*)phys, 0, PAGE_SIZE);

	return phys;
}

static void alloc_init_pte(pmd_t* pmdp, unsigned long long addr,
	unsigned long long end, unsigned long long phys,
	unsigned long long prot,
	unsigned long long (*alloc_pgtable)(void),
	unsigned long long flags)
{
	pmd_t pmd = *pmdp;
	pte_t* ptep;

	if(pmd_none(pmd)){
		unsigned long long pte_phys;

		pte_phys = alloc_pgtable();
		set_pmd(pmdp, __pmd(pte_phys | PMD_TYPE_TABLE));
		pmd = *pmdp; 
	}

	ptep = pte_offset_phys(pmdp,addr);
	do{
		set_pte(ptep, pfn_pte(phys >> PAGE_SHIFT,prot));
		phys += PAGE_SIZE;
	}while(ptep++, addr += PAGE_SIZE, addr != end);
}

static void pmd_set_section(pmd_t *pmdp, unsigned long phys,
		unsigned long prot)
{
	unsigned long sect_prot = PMD_TYPE_SECT | mk_sect_prot(prot);

	pmd_t new_pmd = pfn_pmd(phys >> PMD_SHIFT, sect_prot);

	set_pmd(pmdp, new_pmd);
}

static void alloc_init_pmd(pud_t* pudp, unsigned long long addr,
	unsigned long long end, unsigned long long phys,
	unsigned long long prot,
	unsigned long long (*alloc_pgtable)(void),
	unsigned long long flags)
{
	pud_t pud = *pudp;
	pmd_t* pmdp;
	unsigned long long next;

	if(pud_none(pud)){
		unsigned long long pmd_phys;
		pmd_phys = alloc_pgtable();
		set_pud(pudp, __pud(pmd_phys | PUD_TYPE_TABLE));
		pud = *pudp;
	}		

	pmdp = pmd_offset_phys(pudp,addr);
	do {
		next = pmd_addr_end(addr, end);

		if(((addr | next | phys) & ~SECTION_MASK) == 0 &&
			(flags & NO_BLOCK_MAPPINGS) == 0){
			pmd_set_section(pmdp,phys,prot);
		}
		else{
			alloc_init_pte(pmdp,addr,next,phys,
				prot, alloc_pgtable,flags);
		}

		phys += next - addr;

	}while(pmdp++, addr = next, addr != end);
}


static void alloc_init_pud(pgd_t* pgdp, unsigned long long addr,
	unsigned long long end, unsigned long long phys,
	unsigned long long prot,
	unsigned long long (*alloc_pgtable)(void),
	unsigned long long flags)
{
	pgd_t pgd = *pgdp;
	pud_t *pudp;
	unsigned long long next;

	if(pgd_none(pgd)){
		unsigned long long pud_phys;
		pud_phys = alloc_pgtable();
		set_pgd(pgdp, __pgd(pud_phys | PUD_TYPE_TABLE));
		pgd = *pgdp;
	}

	pudp = pud_offset_phys(pgdp, addr);

	do{
		next = pud_addr_end(addr,end);
		alloc_init_pmd(pudp, addr, next, phys,
			prot, alloc_pgtable, flags);

		phys += next - addr;

	}while(pudp++, addr = next, addr != end);
}


static void __create_pgd_mapping(pgd_t* pgdir, unsigned long long phys,
	unsigned long long virt, unsigned long long size,
	unsigned long long prot,
	unsigned long long (*alloc_pgtable)(void),
	unsigned long long flags)
{
	pgd_t* pgdp = pgd_offset_raw(pgdir,virt);

	unsigned long long addr, end, next;

	phys &= PAGE_MASK;
	addr = virt & PAGE_MASK;
	end = PAGE_ALIGN(virt + size);

	do {

		next = pgd_addr_end(addr, end);
		alloc_init_pud(pgdp, addr, next, phys, 
			prot, alloc_pgtable, flags);
		phys += next - addr;
	}while(pgdp++, addr = next, addr != end);
}

static void create_identical_mapping(void)
{
	unsigned long long start;
	unsigned long long end;
	
#if 0
	start = (unsigned long long)_text_boot_start;
	end = TOTAL_MEMORY;
	__create_pgd_mapping((pgd_t *)idmap_pg_dir,start,
		start,end - start,
		PAGE_KERNEL,
		early_pgtable_alloc,
		0);
#else
	start = (unsigned long long)_text_boot_start;
	end = (unsigned long long)_text_end;

	__create_pgd_mapping((pgd_t *)idmap_pg_dir,start,
		start,end - start,
		PAGE_KERNEL_ROX,
		early_pgtable_alloc,
		0);

	start = PAGE_ALIGN((unsigned long long)_text_end);
	end = TOTAL_MEMORY;
	__create_pgd_mapping((pgd_t *)idmap_pg_dir,start,
		start,end - start,
		PAGE_KERNEL,
		early_pgtable_alloc,
		0);
#endif		
}

static void create_mmio_mapping(void)
{
	__create_pgd_mapping((pgd_t *)idmap_pg_dir,MMIO_BASE,MMIO_BASE,
		MMIO_DEVICE_SIZE,
		PROT_DEVICE_nGnRnE,
		early_pgtable_alloc,
		0);
}

static void cpu_init()
{
	unsigned long long mair = 0;
	unsigned long long tcr = 0;
	unsigned long long tmp = 0;
	unsigned long long parang = 0;

	asm("tlbi vmalle1");
	dsb(nsh);

	mair = MAIR((unsigned long long)0x00, MT_DEVICE_nGnRnE) |
		MAIR((unsigned long long)0x04, MT_DEVICE_nGnRE)	|
		MAIR((unsigned long long)0x0c, MT_DEVICE_GRE) |
		MAIR((unsigned long long)0x44, MT_NORMAL_NC) |
		MAIR((unsigned long long)0xff, MT_NORMAL) |
		MAIR((unsigned long long)0xbb, MT_NORMAL_WT);

	write_sysreg(mair,mair_el1);

	tcr = TCR_TxSZ(VA_BITS) | TCR_TG_FLAGS;

	tmp = read_sysreg(ID_AA64MMFR0_EL1);
	parang = tmp & 0xf;
	if(parang > ID_AA64MMFR0_PARANGE_48)
		parang = ID_AA64MMFR0_PARANGE_48;

	tcr |= parang << TCR_IPS_SHIFT;

	write_sysreg(tcr, tcr_el1);
}

static int enable_mmu()
{
	unsigned long long tmp;
	int tgran4;

	tmp = read_sysreg(ID_AA64MMFR0_EL1);
	tgran4 = (tmp >> ID_AA64MMFR0_TGRAN4_SHIFT) & 0xf;

	if(tgran4 != ID_AA64MMFR0_TGRAN4_SUPPORTED)
		return -1;

	write_sysreg(idmap_pg_dir, ttbr0_el1);
	isb();

	write_sysreg(SCTLR_ELx_M, sctlr_el1);
	isb();

	asm("ic iallu");
	dsb(nsh);
	isb();

	return 0;
}

void page_init()
{
	memset(idmap_pg_dir, 0, PAGE_SIZE);

	create_identical_mapping();
	create_mmio_mapping();
	cpu_init();
	enable_mmu();

	printf("\renable mmu done.\n");	
}

pte_t *walk_pgtable(unsigned long long address)
{
	pgd_t *pgd = NULL;
	pud_t *pud = NULL;
	pte_t *pte = NULL;
	pmd_t *pmd = NULL; 

	pgd = pgd_offset_raw((pgd_t*) idmap_pg_dir, address);
	if(pgd == NULL || pgd_none(*pgd))
		return NULL;

	pud = pud_offset_phys(pgd,address);
	if(pud == NULL || pud_none(*pud))
		return NULL;

	pmd = pmd_offset_phys(pud,address);

	if(pmd == NULL || pmd_none(*pmd)){
		return NULL;
	}else if ((pmd_val(*pmd) & PMD_TYPE_MASK) == PMD_TYPE_SECT){
		//Section
		return (pte_t*)pmd;
	}

	pte = pte_offset_phys(pmd, address);

	if(pte == NULL || pte_none(*pte))
		return NULL;

	return pte;
}

static const char *mt_type_str[] = {
	[0 ... MT_MAX] = "UNKNOWN",
	[MT_DEVICE_nGnRnE] =	"DEVICE/nGnRnE",
	[MT_DEVICE_nGnRE] =		"DEVICE/nGnRE",
	[MT_DEVICE_GRE] = 		"DEVICE/GRE",
	[MT_NORMAL_NC] = 		"MEMORY/NC",
	[MT_NORMAL] = 			"MEMORY/NORMAL",
	[MT_NORMAL_WT] =		"MEMORY/WT",
};

void page_dump()
{
	unsigned long long start = 0;
	unsigned long long end = 0; 
	unsigned long long dump_end = TOTAL_MEMORY > (MMIO_BASE + MMIO_DEVICE_SIZE) ?
		TOTAL_MEMORY : (MMIO_BASE + MMIO_DEVICE_SIZE);
	pte_t * pte = NULL;

	printf("\r---[ Identical mapping ]---\n");

	while(end != dump_end)
	{
		pte = walk_pgtable(start);	

		if(pte != NULL)
		{
			bool is_page = true;
			bool is_read_only = (pte_val(*pte) & PTE_RDONLY) != 0;
			bool is_xn = (pte_val(*pte) & PTE_PXN) != 0;
			bool is_shared = (pte_val(*pte) & PTE_SHARED) != 0;
			bool is_af = (pte_val(*pte) & PTE_AF) != 0;
			bool is_uxn = (pte_val(*pte) & PTE_UXN) != 0;
			int type = (pte_val(*pte) & PTE_ATTRINDX_MASK) >> 2;

			if((pte_val(*pte) & PTE_TYPE_MASK) != PTE_TYPE_PAGE)
			{	
				is_page = false;
				end = start + SECTION_SIZE;
			}
			else
				end = start + PAGE_SIZE;

			printf("\r0x%llx - 0x%llx    %s PTE    %s %s %s %s    %s %s\n",
				start, end,
				is_page ? "4K" : "2MB",
				is_read_only ? "ro" : "RW",
				is_xn ? "NX" : "x",
				is_shared ? "SHD" : "NSH",
				is_af ? "AF" : "NA",
				is_uxn ? "UXN": "UX ",
				mt_type_str[type]);			
		}
		else
		{
			start += PAGE_SIZE;
			continue;
		}
		start = end;
	}
}