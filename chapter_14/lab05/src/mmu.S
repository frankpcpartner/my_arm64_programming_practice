#define _ASM_
#include "page.h"
#include "sysregs.h"

.section ".text"
.global page_init_asm
 
.align 3
page_init_asm:
	stp x29, x30, [sp, #-16]!
	mov x29, sp

	adrp x0, idmap_pg_dir
	add x0, x0, #:lo12:idmap_pg_dir
	mov x1, 0
	ldr x2, =PAGE_SIZE
	bl memset
	
	bl create_identical_mapping_asm
	bl create_mmio_mapping_asm
	bl cpu_init_asm
	bl enable_mmu_asm

	adr x0, str_enable_mmu_dont_fmt
	bl _print

	ldp x29, x30, [sp], #16
	ret

//SP: x29, x30, start, end
//	  0  , 8  , 16   , 24	
create_identical_mapping_asm:
	stp x29, x30, [sp, #-32]!
	mov x29, sp

	//Mapping text section
	adrp x0, _text_boot_start
	add x0, x0, #:lo12:_text_boot_start
	str x0, [sp, #16]

	adrp x0, _text_end
	add x0, x0, #:lo12:_text_end
	str x0, [sp, #24]

	adrp x0, idmap_pg_dir
	add x0, x0, #:lo12:idmap_pg_dir

	ldr x1, [sp, #16]
	ldr x2, [sp, #16]

	ldr x3, [sp, #24]
	sub x3, x3, x1

	ldr x4, =PAGE_KERNEL_ROX

	adrp x5, early_pgtable_alloc
	add x5, x5, #:lo12:early_pgtable_alloc 

	mov x6, #0

	bl __create_pgd_mapping

	
	adrp x0, _text_end
	add x0, x0, #:lo12:_text_end
	bl page_align_asm
	str x0, [sp, #16]

	mov x0, #TOTAL_MEMORY
	str x0, [sp, #24]
	
	adrp x0, idmap_pg_dir
	add x0, x0, #:lo12:idmap_pg_dir

	ldr x1, [sp, #16]
	ldr x2, [sp, #16]

	ldr x3, [sp, #24]
	sub x3, x3, x1

	ldr x4, =PAGE_KERNEL

	adrp x5, early_pgtable_alloc
	add x5, x5, #:lo12:early_pgtable_alloc 

	mov x6, #0

	bl __create_pgd_mapping

	ldp x29, x30, [sp], #32
	ret


//SP: x29, x30, pgdir, phys, virt, size, prot, alloc_pgtable, flags, pgdp, addr, end, next, x   
//    0  , 8  , 16,    24  , 32  , 40  , 48  , 56           , 64   , 72  , 80  , 88 , 96  , 104     
__create_pgd_mapping:
	stp x29, x30, [sp, #-112]!
	mov x29, sp

	str x0, [sp, #16]
	str x1, [sp, #24]
	str x2, [sp, #32]
	str x3, [sp, #40]
	str x4, [sp, #48]
	str x5, [sp, #56]
	str x6, [sp, #64]
	
	ldr x0, [sp, #16]
	ldr x1, [sp, #32]
	bl pgd_offset_raw_asm
	str x0, [sp, #72]

	ldr x0, [sp, #24]
	and x0, x0, #PAGE_MASK
	str x0, [sp, #24]

	ldr x0, [sp, #32]
	and x0, x0, #PAGE_MASK
	str x0, [sp, #80]
 
 	ldr x0, [sp, #32]
	ldr x1, [sp, #40]
	add x0, x0, x1
	bl page_align_asm
	str x0, [sp, #88]

1:	ldr x0, [sp, #80]
	ldr x1, [sp, #88]
	bl pgd_addr_end_asm
	str x0, [sp, #96]

	ldr x0, [sp, #72]
	ldr x1, [sp, #80]
	ldr x2, [sp, #96]
	ldr x3, [sp, #24]
	ldr x4, [sp, #48]
	ldr x5, [sp, #56]
	ldr x6, [sp, #64]

	bl alloc_init_pud_asm

	ldr x0, [sp, #96]
	ldr x1, [sp, #80]
	sub x0, x0, x1	
	ldr x1, [sp, #24]
	add x1, x1, x0
	str x1, [sp, #24]

 	ldr x0, [sp, #72]
 	add x0, x0, #8
 	str x0, [sp, #72]

 	ldr x0, [sp, #96]
 	str x0, [sp, #80]
	
	ldr x0, [sp, #80]
	ldr x1, [sp, #88]
	cmp x0, x1
	b.ne 1b 	

	ldp x29, x30, [sp], #112
	ret


create_mmio_mapping_asm:
	stp x29, x30, [sp, #-16]!
	mov x29, sp

	adrp x0, idmap_pg_dir
	add x0, x0, #:lo12:idmap_pg_dir 

	mov x1, #MMIO_BASE
	mov x2, #MMIO_BASE
	mov x3, #MMIO_DEVICE_SIZE
	ldr x4, =PROT_DEVICE_nGnRnE

	adrp x5, early_pgtable_alloc
	add x5, x5, #:lo12:early_pgtable_alloc

	mov x6, 0

	bl  __create_pgd_mapping

	ldp x29, x30, [sp],#16
	ret


//sp: x29, x30, mair, tcr, tmp, parang
//    0  , 8  , 16  , 24 , 32 , 40 
cpu_init_asm:
	stp x29, x30, [sp, #-48]!	
	mov x29, sp

	str xzr, [sp, #16]
	str xzr, [sp, #24]
	str xzr, [sp, #32]
	str xzr, [sp, #40]

	tlbi vmalle1
	dsb	nsh

	mov	x0, #0x400
	movk x0, #0x440c, lsl #16
	movk x0, #0xbbff, lsl #32	
	str	x0, [sp, #16]

	ldr x0, [sp, #16]
	msr mair_el1, x0

	mov	x0, #0x10 
	movk x0, #0x8010, lsl #16
	str	x0, [sp, #24]

	mrs x0, ID_AA64MMFR0_EL1
	str x0, [sp, #32]

	and x0, x0, #0xf
	str x0, [sp, #40]

	cmp x0, #0x5	
	b.ls 1f
	mov x0, #0x5
	str x0, [sp, #40]	

1:	ldr x0, [sp, #24]
	ldr x1, [sp, #40]
	lsl	x1, x1, #32
	orr x0, x0, x1
	str x0, [sp, #24]
	
	msr tcr_el1, x0 
	
	ldp x29, x30, [sp], #48
	ret

//sp: tmp, tgran4
//    0  , 8 
enable_mmu_asm:
	sub sp, sp, #16

	mrs x0, ID_AA64MMFR0_EL1
	str x0, [sp]

	lsr	x0, x0, #28
	and x0, x0, #0xf	
	str x0, [sp, #8]
	
	cmp x0, #0
	b.eq 1f
	mov x0, #-1
	b 2f

1:	adrp x0, idmap_pg_dir
	add x0, x0, #:lo12:idmap_pg_dir
	msr ttbr0_el1, x0
	isb

	ldr x0, =SCTLR_ELx_M
	msr sctlr_el1, x0
	isb

	ic iallu

	dsb nsh
	
	isb

	mov x0, #0
2:
	add sp, sp, #16
	ret	

pgd_page_paddr_asm:
	and	x0, x0, #0xfffffffff000
	ret

pud_page_paddr_asm:
	and	x0, x0, #0xfffffffff000
	ret

pmd_page_paddr_asm:
	and	x0, x0, #0xfffffffff000
	ret

//X0:pgd
//X1:virtual address
pgd_offset_raw_asm:
	lsr x1, x1, #39
	and	x1, x1, #0x1ff
	lsl	x1, x1, #3
	add x0, x0, x1
	ret

//sp: x29, x30, pgd, addr
//	  0	 , 8  , 16 , 24  
pud_offset_phys_asm:
	stp x29, x30, [sp, #-32]!
	mov x29, sp

	str x0, [sp, #16]
	str x1, [sp, #24]

	ldr x0, [sp, #16]
	ldr x0, [x0]
	bl pgd_page_paddr_asm

	ldr x1, [sp, #24]
	lsr x1, x1, #30
	and x1, x1, #0x1ff
	lsl	x1, x1, #3
	add x0, x0, x1

	ldp x29, x30, [sp], #32
	ret

//sp: x29, x30, pud, addr
//	  0	 , 8  , 16 , 24  
pmd_offset_phys_asm:
	stp x29, x30, [sp, #-32]!
	mov x29, sp

	str x0, [sp, #16]
	str x1, [sp, #24]

	ldr x0, [sp, #16]
	ldr x0, [x0]
	bl pud_page_paddr_asm

	ldr x1, [sp, #24]
	lsr x1, x1, #21
	and x1, x1, #0x1ff
	lsl	x1, x1, #3
	add x0, x0, x1

	ldp x29, x30, [sp], #32
	ret

//sp: x29, x30, pmd, addr
//	  0	 , 8  , 16 , 24  
pte_offset_phys_asm:
	stp x29, x30, [sp, #-32]!
	mov x29, sp

	str x0, [sp, #16]
	str x1, [sp, #24]

	ldr x0, [sp, #16]
	ldr x0, [x0]
	bl pmd_page_paddr_asm

	ldr x1, [sp, #24]
	lsr x1, x1, #12
	and x1, x1, #0x1ff
	lsl	x1, x1, #3
	add x0, x0, x1

	ldp x29, x30, [sp], #32
	ret

//x0: virtual address
page_align_asm:
	add x0, x0, #0xfff
	and x0, x0, #0xfffffffffffff000
	ret	

//x0: virtual address
//x1: end address
pgd_addr_end_asm:
	//x0:boundary, x1:end
	ldr x4, =PGDIR_SIZE
	add x0, x0, x4
	and x0, x0, #0xffffff8000000000
	mov x2, x0
	mov x3, x1
	sub x2, x2, #1
 	sub x3, x3, #1
 	cmp x2, x3
 	b.ls 1f
 	mov x0, x1
 1:	
 	ret

//x0: virtual address
//x1: end address
pud_addr_end_asm:
	//x0:boundary, x1:end
	ldr x4, =PUD_SIZE
	add x0, x0, x4
	and x0, x0, #PUD_MASK
	mov x2, x0
	mov x3, x1
	sub x2, x2, #1
 	sub x3, x3, #1
 	cmp x2, x3
 	b.ls 1f
 	mov x0, x1
 1:	
 	ret

//x0: virtual address
//x1: end address
pmd_addr_end_asm:
	//x0:boundary, x1:end
	add x0, x0, #PMD_SIZE 
	and x0, x0, #PMD_MASK
	mov x2, x0
	mov x3, x1
	sub x2, x2, #1
 	sub x3, x3, #1
 	cmp x2, x3
 	b.ls 1f
 	mov x0, x1
 1:	
 	ret


//sp: x29, x30, pgdp, addr, end, phys, prot, alloc_pgtable, flags, pgd, pudp, next, pud_phys, none
//    0  , 8  , 16  , 24  , 32 , 40  , 48  , 56           , 64   , 72 , 80  , 88  , 96      , 104
alloc_init_pud_asm:
	stp x29, x30, [sp, #-112]!
	mov x29, sp 	

	str x0, [sp, #16]
	str x1, [sp, #24]
	str x2, [sp, #32]
	str x3, [sp, #40]
	str x4, [sp, #48]
	str x5, [sp, #56]
	str x6, [sp, #64]

	ldr x0, [sp, #16]
	ldr x0, [x0]
	str x0, [sp, 72]

	cmp x0, #0
	b.ne 1f

	ldr x0, [sp, #56]
	blr x0
	str x0, [sp, #96]
	ldr x1, [sp, #16]
	orr x0, x0, #0x3
	str x0, [x1]
	dsb	ishst
	str x0, [sp, #72]	

1:	ldr x0, [sp, #16]
	ldr x1, [sp, #24]
	bl pud_offset_phys_asm
	str x0, [sp, #80]

2:	ldr x0, [sp, #24]
	ldr x1, [sp, #32]
	bl pud_addr_end_asm
	str x0, [sp, #88]

	ldr x0, [sp, #80]
	ldr x1, [sp, #24]
	ldr x2, [sp, #88]
	ldr x3, [sp, #40]
	ldr x4, [sp, #48]
	ldr x5, [sp, #56]
	ldr x6, [sp, #64]

	bl alloc_init_pmd_asm

	str x0, [sp, #88]
	str x1, [sp, #24]
	str x2, [sp, #40]
	sub x0, x0, x1
	add x2, x2, x0
	str x2, [sp, #40]

	ldr x0, [sp, #80]
	add x0, x0, #8
	str x0, [sp, #80]

	ldr x0, [sp, #88]
	str x0, [sp, #24]

	ldr x1, [sp, #32]
	cmp x0, x1

	b.ne 2b 

	ldp x29, x30, [sp], #112
	ret

//sp: x29, x30, pudp, addr, end, phys, prot, alloc_pgtable, flags, pud, pmdp, next, pmd_phys,    
//    0  , 8  , 16  , 24  , 32 , 40  , 48  , 56           , 64   , 72 , 80  , 88  , 96      , 104
alloc_init_pmd_asm:
	stp x29, x30, [sp, #-112]!
	mov x29, sp

	str x0, [sp, #16]
	str x1, [sp, #24]
	str x2, [sp, #32]
	str x3, [sp, #40]
	str x4, [sp, #48]
	str x5, [sp, #56]
	str x6, [sp, #64]

	ldr x0, [sp, #16]
	ldr x1, [x0]
	str x1, [sp, #72]

	ldr x0, [sp, #72]
	cmp x0, #0
	b.ne 1f

	ldr x0, [sp, #56]
	blr x0
	str x0, [sp, #96]
	orr x0, x0, #3
	ldr x1, [sp, #16]
	str x0, [x1]
	dsb	ishst
	str x0, [sp, #72]
	
1:	ldr x0, [sp, #16]
	ldr x1, [sp, #24]
	bl pmd_offset_phys_asm
	str x0, [sp, #80]

2:	ldr x0, [sp, #24]
	ldr x1, [sp, #32]
	bl pmd_addr_end_asm
	str x0, [sp, #88]

	ldr x0, [sp, #24]
	ldr x1, [sp, #88]
	ldr x2, [sp, #40]
	orr x0, x0, x1	
	orr x0, x0, x2
	and x0, x0, #0x1fffff
	cmp x0, #0	
	b.ne 3f
	ldr x0, [sp, #64]
	and	x0, x0, #0x1
	cmp x0, #0
	b.ne 3f

	ldr x0, [sp, #80]
	ldr x1, [sp, #40]
	ldr x2, [sp, #48]
	bl pmd_set_section_asm
	b 4f

3:	ldr	x0, [sp, #80] 
	ldr	x1, [sp, #24]
	ldr	x2, [sp, #88]
	ldr	x3, [sp, #40]
	ldr	x4, [sp, #48]
	ldr	x5, [sp, #56]
	ldr	x6, [sp, #64]

	bl alloc_init_pte_asm


4:	ldr x0, [sp, #88]	
	ldr x1, [sp, #24]	
	ldr x2, [sp, #40]	
	sub x0, x0, x1
	add x2, x2, x0
	str x2, [sp, #40]	

	ldr x0, [sp, #80]
	add x0, x0, #8
	str x0, [sp, #80]

	ldr x0, [sp, #88]
	str x0, [sp, #24]

	ldr x1, [sp, #32]
	cmp x0, x1

	b.ne 2b

	ldp x29, x30, [sp], #112
	ret

//sp:x29, x30, pmdp, addr, end, phys, prot, alloc_pgtable, flags, pmd, ptep, pte_phys,  
//   0  , 8  , 16  , 24  , 32 , 40  , 48  , 56           , 64   , 72 , 80  , 88       
alloc_init_pte_asm:
	stp x29, x30, [sp, #-96]!
	mov x29, sp

	str x0, [sp, #16]
	str x1, [sp, #24]
	str x2, [sp, #32]
	str x3, [sp, #40]
	str x4, [sp, #48]
	str x5, [sp, #56]
	str x6, [sp, #64]
	
	ldr x0, [sp, #16]
	ldr x0, [x0]
	str x0, [sp, #72]
	
	cmp x0, #0
	b.ne 1f

	ldr x0, [sp, #56]
	blr x0
	str x0, [sp, #88]

	orr x0, x0, #3
	mov x1, x0
	ldr x0, [sp, #16]
	str x1, [x0]
	dsb	ishst

	str x1, [sp, #72]

1:	ldr x0, [sp, #16]
	ldr x1, [sp, #24]
	bl pte_offset_phys_asm
	str x0, [sp, #80]

2:	ldr x0, [sp, #40]
	and x1, x0, 0xfffffffffffff000
	ldr x0, [sp, #48]
	orr x1, x1, x0
	ldr x0, [sp, #80]
	str x1, [x0]

	ldr x0, [sp, #40]
	add x0, x0, #PAGE_SIZE
	str x0, [sp, #40]

	ldr x0, [sp, #80]
	add x0, x0, #8
	str x0, [sp, #80]

	ldr x0, [sp, #24]
	add x0, x0, #PAGE_SIZE
	str x0, [sp, #24]

	ldr x1, [sp, #32]
	cmp x0, x1
	b.ne 2b

	ldp x29, x30, [sp], #96
	ret

//sp: x29, x30, pmdp, phys, prot, sect_prot, new_pmd
//    0  , 8  , 16  , 24  , 32  , 40       , 48
pmd_set_section_asm:
	stp x29, x30, [sp, #-64]!
	mov x29, sp

	str x0, [sp, #16]
	str x1, [sp, #24]
	str x2, [sp, #32]

	ldr x0, [sp, #32]
	and	x0, x0, #0xfffffffffffffffd
	orr x0, x0, #0x1
	
	str x0, [sp, #40]

	ldr x0, [sp, #24]
	and	x0, x0, #0xffffffffffe00000
	ldr x1, [sp, #40]
	orr x0, x0, x1
	str x0, [sp, #48]

	ldr x0, [sp, #16]
	ldr x1, [sp, #48]
	str x1, [x0]

	dsb	ishst

	ldp x29, x30, [sp], #64
	ret

.section .rodata
.align 3
str_enable_mmu_dont_fmt:
	.string "\renable mmu done.\n"
