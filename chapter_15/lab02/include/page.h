#ifndef _PAGE_H
#define _PAGE_H

#include "core.h"
#include "barrier.h"

#define VA_BITS			48

/*PGD LV0*/

#define PGDIR_SHIFT		39
#define PGDIR_SIZE		(1ULL << PGDIR_SHIFT)
#define PGDIR_MASK		(~(PGDIR_SIZE-1))
#define PTRS_PER_PGD	(1 << (VA_BITS - PGDIR_SHIFT))

/*PUD LV1*/
#define PUD_SHIFT		30
#define PUD_SIZE		(1ULL << PUD_SHIFT)
#define PUD_MASK		(~(PUD_SIZE-1))
#define PTRS_PER_PUD	(1 << (PGDIR_SHIFT - PUD_SHIFT))

/*PMD LV2*/
#define PMD_SHIFT		21
#define PMD_SIZE		(1ULL << PMD_SHIFT)
#define PMD_MASK		(~(PMD_SIZE-1))
#define PTRS_PER_PMD	(1ULL << (PUD_SHIFT - PMD_SHIFT))

/*PTE*/
#define PTE_SHIFT		12
#define PTE_SIZE		(1ULL << PTE_SHIFT)
#define PTE_MASK		(~(PTE_SIZE-1))
#define PTRS_PER_PTE	(1 << (PMD_SHIFT - PTE_SHIFT))

/* Section */
#define SECTION_SHIFT	PMD_SHIFT
#define SECTION_SIZE	(1ULL << SECTION_SHIFT)
#define SECTION_MASK	(~(SECTION_SIZE-1))

#define PTE_TYPE_MASK		(3ULL << 0)
#define PTE_TYPE_FAULT		(0ULL << 0)
#define PTE_TYPE_PAGE		(3ULL << 0)
#define PTE_TABLE_BIT		(1ULL << 1)
#define PTE_USER		(1ULL << 6)		/* AP[1] */
#define PTE_RDONLY		(1ULL << 7)		/* AP[2] */
#define PTE_SHARED		(3ULL << 8)		/* SH[1:0], inner shareable */
#define PTE_AF			(1ULL << 10)	/* Access Flag */
#define PTE_NG			(1ULL << 11)	/* nG */
#define PTE_DBM			(1ULL << 51)	/* Dirty Bit Management */
#define PTE_CONT		(1ULL << 52)	/* Contiguous range */
#define PTE_PXN			(1ULL << 53)	/* Privileged XN */
#define PTE_UXN			(1ULL << 54)	/* User XN */
#define PTE_HYP_XN		(1ULL << 54)	/* HYP XN */

#define PTE_ADDR_LOW (((1ULL << (48 - PAGE_SHIFT)) - 1) << PAGE_SHIFT)
#define PTE_ADDR_MASK PTE_ADDR_LOW

/*
 * Hardware page table definitions.
 *
 * Level 1 descriptor (PUD).
 */
#define PUD_TYPE_TABLE		(3ULL << 0)
#define PUD_TABLE_BIT		(1ULL << 1)
#define PUD_TYPE_MASK		(3ULL << 0)
#define PUD_TYPE_SECT		(1ULL << 0)


/*
 * Level 2 descriptor (PMD).
 */
#define PMD_TYPE_MASK		(3ULL << 0)
#define PMD_TYPE_FAULT		(0ULL << 0)
#define PMD_TYPE_TABLE		(3ULL << 0)
#define PMD_TYPE_SECT		(1ULL << 0)
#define PMD_TABLE_BIT		(1ULL << 1)


/*
 * Memory types available.
 */
#define MT_DEVICE_nGnRnE	0
#define MT_DEVICE_nGnRE		1
#define MT_DEVICE_GRE		2
#define MT_NORMAL_NC		3
#define MT_NORMAL			4
#define MT_NORMAL_WT		5
#define MT_MAX				5

#define MAIR(attr, mt)	((attr) << ((mt) * 8))

/*
 * AttrIndx[2:0] encoding (mapping attributes defined in the MAIR* registers).
 */
#define PTE_ATTRINDX(t)		((t) << 2)
#define PTE_ATTRINDX_MASK	(7 << 2)

#define PTE_VALID		(1ULL << 0)
#define PTE_WRITE		(PTE_DBM)		 /* same as DBM (51) */
#define PTE_DIRTY		(1ULL << 55)
#define PTE_SPECIAL		(1ULL << 56)
#define PTE_PROT_NONE		(1ULL << 58) /* only when !PTE_VALID */

#define _PROT_DEFAULT		(PTE_TYPE_PAGE | PTE_AF | PTE_SHARED)
#define PROT_DEFAULT (_PROT_DEFAULT)

#define PROT_DEVICE_nGnRnE	(PROT_DEFAULT | PTE_PXN | PTE_UXN | PTE_DIRTY | PTE_WRITE | PTE_ATTRINDX(MT_DEVICE_nGnRnE))
#define PROT_DEVICE_nGnRE	(PROT_DEFAULT | PTE_PXN | PTE_UXN | PTE_DIRTY | PTE_WRITE | PTE_ATTRINDX(MT_DEVICE_nGnRE))
#define PROT_NORMAL_NC		(PROT_DEFAULT | PTE_PXN | PTE_UXN | PTE_DIRTY | PTE_WRITE | PTE_ATTRINDX(MT_NORMAL_NC))
#define PROT_NORMAL_WT		(PROT_DEFAULT | PTE_PXN | PTE_UXN | PTE_DIRTY | PTE_WRITE | PTE_ATTRINDX(MT_NORMAL_WT))
#define PROT_NORMAL		(PROT_DEFAULT | PTE_PXN | PTE_UXN | PTE_DIRTY | PTE_WRITE | PTE_ATTRINDX(MT_NORMAL))

#define PAGE_KERNEL_RO		((PROT_NORMAL & ~PTE_WRITE) | PTE_RDONLY)
#define PAGE_KERNEL_ROX		((PROT_NORMAL & ~(PTE_WRITE | PTE_PXN)) | PTE_RDONLY)
#define PAGE_KERNEL_EXEC	(PROT_NORMAL & ~PTE_PXN)

#define PAGE_KERNEL PROT_NORMAL

#define TOTAL_MEMORY (512 * 0x100000)	

#define PAGE_SHIFT	 		12
#define PAGE_SIZE   		(1 << PAGE_SHIFT)	
#define PAGE_MASK 			(~(PAGE_SIZE-1))

/* to align the pointer to the (next) page boundary */
#define PAGE_ALIGN(addr) (((addr)+PAGE_SIZE-1)&PAGE_MASK)

#define pgd_none(pgd) (!pgd_val(pgd))
#define pud_none(pud) (!pud_val(pud))
#define pmd_none(pmd) (!pmd_val(pmd))
#define pte_none(ptd) (!pte_val(ptd))

/* 查找PGD索引 */
#define pgd_index(addr) (((addr) >> PGDIR_SHIFT) & (PTRS_PER_PGD - 1))

/* 通过地址addr查找PGD的表项 */
#define pgd_offset_raw(pgd, addr) ((pgd) + pgd_index(addr))

#define pgd_addr_end(addr, end)						\
({	unsigned long long __boundary = ((addr) + PGDIR_SIZE) & PGDIR_MASK;	\
	(__boundary - 1 < (end) - 1) ? __boundary : (end);		\
})

#define pud_addr_end(addr, end)						\
({	unsigned long long __boundary = ((addr) + PUD_SIZE) & PUD_MASK;	\
	(__boundary - 1 < (end) - 1) ? __boundary : (end);		\
})

#define pmd_addr_end(addr, end)						\
({	unsigned long long __boundary = ((addr) + PMD_SIZE) & PMD_MASK;	\
	(__boundary - 1 < (end) - 1) ? __boundary : (end);		\
})


#define pud_index(addr) ((addr) >> PUD_SHIFT & (PTRS_PER_PUD - 1))
#define pud_offset_phys(pgd, addr) ((pud_t *)((pgd_page_paddr(*(pgd)) + pud_index(addr) * sizeof(pud_t))))

#define pmd_index(addr) ((addr) >> PMD_SHIFT & (PTRS_PER_PMD - 1))
#define pmd_offset_phys(pud, addr) ((pmd_t *)(pud_page_paddr(*(pud)) + pmd_index(addr) * sizeof(pmd_t)))

#define pfn_pmd(pfn, prot) (__pmd(((unsigned long long)(pfn) << PMD_SHIFT) | (prot)))


#define pte_index(addr) (((addr) >> PAGE_SHIFT) & (PTRS_PER_PTE - 1))
#define pte_offset_phys(dir, addr) ((pte_t *)(pmd_page_paddr(*(dir)) + pte_index(addr) * sizeof(pte_t)))

#define pfn_pte(pfn, prot)  (__pte(((unsigned long long)(pfn) << PAGE_SHIFT) | (prot)))

#define NR_PAGES (TOTAL_MEMORY / PAGE_SIZE)

#define LOW_MEMORY (2 * SECTION_SIZE) /*4MB*/

/*
 * TCR flags.
 */
#define TCR_T0SZ_OFFSET		0
#define TCR_T1SZ_OFFSET		16
#define TCR_T0SZ(x)		((64ULL - (x)) << TCR_T0SZ_OFFSET)
#define TCR_T1SZ(x)		((64ULL - (x)) << TCR_T1SZ_OFFSET)
#define TCR_TxSZ(x)		(TCR_T0SZ(x) | TCR_T1SZ(x))
#define TCR_TxSZ_WIDTH		6
#define TCR_T0SZ_MASK		(((1ULL << TCR_TxSZ_WIDTH) - 1) << TCR_T0SZ_OFFSET)

#define TCR_EPD0_SHIFT		7
#define TCR_EPD0_MASK		(1ULL << TCR_EPD0_SHIFT)
#define TCR_IRGN0_SHIFT		8
#define TCR_IRGN0_MASK		(3ULL << TCR_IRGN0_SHIFT)
#define TCR_IRGN0_NC		(0ULL << TCR_IRGN0_SHIFT)
#define TCR_IRGN0_WBWA		(1ULL << TCR_IRGN0_SHIFT)
#define TCR_IRGN0_WT		(2ULL << TCR_IRGN0_SHIFT)
#define TCR_IRGN0_WBnWA		(3ULL << TCR_IRGN0_SHIFT)

#define TCR_EPD1_SHIFT		23
#define TCR_EPD1_MASK		(1ULL << TCR_EPD1_SHIFT)
#define TCR_IRGN1_SHIFT		24
#define TCR_IRGN1_MASK		(3ULL << TCR_IRGN1_SHIFT)
#define TCR_IRGN1_NC		(0ULL << TCR_IRGN1_SHIFT)
#define TCR_IRGN1_WBWA		(1ULL << TCR_IRGN1_SHIFT)
#define TCR_IRGN1_WT		(2ULL << TCR_IRGN1_SHIFT)
#define TCR_IRGN1_WBnWA		(3ULL << TCR_IRGN1_SHIFT)

#define TCR_IRGN_NC		(TCR_IRGN0_NC | TCR_IRGN1_NC)
#define TCR_IRGN_WBWA		(TCR_IRGN0_WBWA | TCR_IRGN1_WBWA)
#define TCR_IRGN_WT		(TCR_IRGN0_WT | TCR_IRGN1_WT)
#define TCR_IRGN_WBnWA		(TCR_IRGN0_WBnWA | TCR_IRGN1_WBnWA)
#define TCR_IRGN_MASK		(TCR_IRGN0_MASK | TCR_IRGN1_MASK)


#define TCR_ORGN0_SHIFT		10
#define TCR_ORGN0_MASK		(3ULL << TCR_ORGN0_SHIFT)
#define TCR_ORGN0_NC		(0ULL << TCR_ORGN0_SHIFT)
#define TCR_ORGN0_WBWA		(1ULL << TCR_ORGN0_SHIFT)
#define TCR_ORGN0_WT		(2ULL << TCR_ORGN0_SHIFT)
#define TCR_ORGN0_WBnWA		(3ULL << TCR_ORGN0_SHIFT)

#define TCR_ORGN1_SHIFT		26
#define TCR_ORGN1_MASK		(3ULL << TCR_ORGN1_SHIFT)
#define TCR_ORGN1_NC		(0ULL << TCR_ORGN1_SHIFT)
#define TCR_ORGN1_WBWA		(1ULL << TCR_ORGN1_SHIFT)
#define TCR_ORGN1_WT		(2ULL << TCR_ORGN1_SHIFT)
#define TCR_ORGN1_WBnWA		(3ULL << TCR_ORGN1_SHIFT)

#define TCR_ORGN_NC		(TCR_ORGN0_NC | TCR_ORGN1_NC)
#define TCR_ORGN_WBWA		(TCR_ORGN0_WBWA | TCR_ORGN1_WBWA)
#define TCR_ORGN_WT		(TCR_ORGN0_WT | TCR_ORGN1_WT)
#define TCR_ORGN_WBnWA		(TCR_ORGN0_WBnWA | TCR_ORGN1_WBnWA)
#define TCR_ORGN_MASK		(TCR_ORGN0_MASK | TCR_ORGN1_MASK)

#define TCR_SH0_SHIFT		12
#define TCR_SH0_MASK		(3ULL << TCR_SH0_SHIFT)
#define TCR_SH0_INNER		(3ULL << TCR_SH0_SHIFT)

#define TCR_SH1_SHIFT		28
#define TCR_SH1_MASK		(3ULL << TCR_SH1_SHIFT)
#define TCR_SH1_INNER		(3ULL << TCR_SH1_SHIFT)
#define TCR_SHARED		(TCR_SH0_INNER | TCR_SH1_INNER)

#define TCR_TG0_SHIFT		14
#define TCR_TG0_MASK		(3ULL << TCR_TG0_SHIFT)
#define TCR_TG0_4K		(0ULL << TCR_TG0_SHIFT)
#define TCR_TG0_64K		(1ULL << TCR_TG0_SHIFT)
#define TCR_TG0_16K		(2ULL << TCR_TG0_SHIFT)

#define TCR_TG1_SHIFT		30
#define TCR_TG1_MASK		(3ULL << TCR_TG1_SHIFT)
#define TCR_TG1_16K		(1ULL << TCR_TG1_SHIFT)
#define TCR_TG1_4K		(2ULL << TCR_TG1_SHIFT)
#define TCR_TG1_64K		(3ULL << TCR_TG1_SHIFT)

#define TCR_IPS_SHIFT		32
#define TCR_IPS_MASK		(7ULL << TCR_IPS_SHIFT)
#define TCR_A1			(1ULL << 22)
#define TCR_ASID16		(1ULL << 36)
#define TCR_TBI0		(1ULL << 37)
#define TCR_TBI1		(1ULL << 38)
#define TCR_HA			(1ULL << 39)
#define TCR_HD			(1ULL << 40)
#define TCR_NFD1		(1 << 54)

#define TCR_TG_FLAGS    (TCR_TG0_4K | TCR_TG1_4K)
#define TCR_KASLR_FLAGS 0
#define TCR_KASAN_FLAGS 0
#define TCR_SMP_FLAGS   TCR_SHARED
#define TCR_CACHE_FLAGS (TCR_IRGN_WBWA | TCR_ORGN_WBWA | TCR_SHARED)


#define ID_AA64MMFR0_TGRAN4_SHIFT	28
#define ID_AA64MMFR0_PARANGE_48		0x5
#define ID_AA64MMFR0_TGRAN4_SUPPORTED	0x0


#define TCR_IPS_SHIFT		32

#define NO_BLOCK_MAPPINGS BIT(0)

#ifndef _ASM_

typedef u64 pteval_t;
typedef u64 pmdval_t;
typedef u64 pudval_t;
typedef u64 pgdval_t;

/*
 * These are used to make use of C type-checking..
 */
typedef struct { pteval_t pte; } pte_t;
#define pte_val(x)	((x).pte)
#define __pte(x)	((pte_t) { (x) } )

typedef struct { pmdval_t pmd; } pmd_t;
#define pmd_val(x)	((x).pmd)
#define __pmd(x)	((pmd_t) { (x) } )

typedef struct { pudval_t pud; } pud_t;
#define pud_val(x)	((x).pud)
#define __pud(x)	((pud_t) { (x) } )

typedef struct { pgdval_t pgd; } pgd_t;
#define pgd_val(x)	((x).pgd)
#define __pgd(x)	((pgd_t) { (x) } )

#define pmd_sect(pmd)	((pmd_val(pmd) & PMD_TYPE_MASK) == \
				 PMD_TYPE_SECT)
#define pud_sect(pud)	((pud_val(pud) & PUD_TYPE_MASK) == \
				 PUD_TYPE_SECT)

static inline unsigned long long pgd_page_paddr(pgd_t pgd)
{
	return pgd_val(pgd) & PTE_ADDR_MASK;
}

static inline void set_pgd(pgd_t *pgdp, pgd_t pgd)
{
	*pgdp = pgd;

	dsb(ishst);
}

static inline void set_pud(pud_t *pudp, pud_t pud)
{
	*pudp = pud;

	dsb(ishst);
}

static inline void set_pmd(pmd_t *pmdp, pmd_t pmd)
{
	*pmdp = pmd;

	dsb(ishst);
}

static inline void set_pte(pte_t *ptep, pte_t pte)
{
	*ptep = pte;
	dsb(ishst);
}

static inline unsigned long long pmd_page_paddr(pmd_t pmd)
{
	return pmd_val(pmd) & PTE_ADDR_MASK;
}

static inline unsigned long long pud_page_paddr(pud_t pud)
{
	return pud_val(pud) & PTE_ADDR_MASK;
}

static inline unsigned long long mk_sect_prot(unsigned long long prot)
{
	return prot & ~PTE_TABLE_BIT;
}


void page_init();
void page_dump();
pte_t *walk_pgtable(unsigned long long address);
unsigned long long early_pgtable_alloc();

unsigned long long get_free_page();
#endif
#endif
