#define _ASM_
#include "sysregs.h"

#define SP_EL2 0x80000
#define SP_EL1 0x60000

.section ".text.boot"
.global _start
_start:
    mrs x1, MPIDR_EL1 
    and x1,x1,#3
    cbz x1, 2f

1:  wfe
    b 1b 


2:  ldr x1, =SP_EL2
    mov sp,x1

    bl _early_uart_init
    bl _printel

    ldr x1, =_bss_start 
    ldr x2, =_bss_end
3:  cmp x1,x2   
    b.eq 4f
    str xzr, [x1], #8
    b 3b

4:  mrs x0, hcr_el2
    ldr x1, =HCR_EL2_RW_AARCH64 
    orr x0, x0, x1
    msr hcr_el2, x0

    mrs x0, sctlr_el1
    ldr x1, =SCTLR_EL1_EE_MASK
    and x0, x0, x1
    ldr x1, =SCTLR_EL1_EE_LITTLE
    orr x0, x0, x1

    ldr x1, =SCTLR_EL1_EOE_MASK
    and x0, x0, x1
    ldr x1, =SCTLR_EL1_EOE_LITTLE
    orr x0, x0, x1

    ldr x1, =SCTLR_EL1_M_MASK
    and x0, x0, x1
    ldr x1, =SCTLR_EL1_M_DISABLE_MMU
    orr x0, x0, x1

    msr sctlr_el1, x0

    mrs x0, spsr_el2
    orr x0, x0, SPSR_EL2_DAIF_DISABLE
    
    ldr x1, =SPSR_EL2_M_MASK
    and x0, x0, x1
    ldr x1, =SPSR_EL2_M_EL1H
    orr x0, x0, x1
    msr spsr_el2, x0

    //Enanle floating point and Advanced SIMD
    mrs x0, cpacr_el1
    ldr x1, =CPACR_EL1_FPEN
    orr x0, x0, x1
    msr cpacr_el1, x0

    adr x0, el1_entry
    msr elr_el2, x0

    eret

el1_entry:
    ldr x0, =SP_EL1
    mov sp, x0 
        
    adrp x0, exception_table
    add x0, x0, #:lo12:exception_table
    msr vbar_el1, x0

    bl _printel    

    bl main 
    b 1b

