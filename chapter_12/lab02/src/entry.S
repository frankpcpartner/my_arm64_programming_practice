#define _ASM_
#include "exception.h"

.section ".text"
	.macro ventry, label
    .align 7  // entry size is 0x80, .align will pad 0
        b   \label
    .endm

    .macro invalid_handle el, reason
    	mov x0, sp
    	mov x1, #\reason
    	mrs x2, esr_el1
    	b bad_mod
    .endm	

.global exception_table
.align 11
exception_table:
	ventry	sync_invalid_el1
	ventry	irq_invalid_el1
	ventry	firq_invalid_el1
	ventry	error_invalid_el1

	ventry	sync_invalid_el1	
	ventry	el1_irq
	ventry	firq_invalid_el1
	ventry	error_invalid_el1

	ventry	sync_invalid_el0
	ventry	irq_invalid_el0
	ventry	firq_invalid_el0
	ventry	error_invalid_el0

	ventry	sync_invalid_el0	
	ventry	irq_invalid_el0
	ventry	firq_invalid_el0
	ventry	error_invalid_el0


sync_invalid_el1:
	invalid_handle 1, BAD_SYNC	

irq_invalid_el1:
	invalid_handle 1, BAD_IRQ

firq_invalid_el1:
	invalid_handle 1, BAD_FIQ

error_invalid_el1:
	invalid_handle 1, BAD_ERROR

sync_invalid_el0:
	invalid_handle 0, BAD_SYNC	

irq_invalid_el0:
	invalid_handle 0, BAD_IRQ

firq_invalid_el0:
	invalid_handle 0, BAD_FIQ

error_invalid_el0:
	invalid_handle 0, BAD_ERROR


el1_irq:
	bl kernel_entry
	bl irq_handle
	bl kernel_exit

kernel_entry:
	sub sp, sp, #S_FRAME_SIZE

    stp x0, x1, [sp, #16* 0]
    stp x2, x3, [sp, #16* 1]
    stp x4, x5, [sp, #16* 2]
    stp x6, x7, [sp, #16* 3]
    stp x8, x9, [sp, #16* 4]
    stp x10, x11, [sp, #16* 5]
    stp x12, x13, [sp, #16* 6]
    stp x14, x15, [sp, #16* 7]
    stp x16, x17, [sp, #16* 8]
    stp x18, x19, [sp, #16* 9]
    stp x20, x21, [sp, #16* 10]
    stp x22, x23, [sp, #16* 11]
    stp x24, x25, [sp, #16* 12]
    stp x26, x27, [sp, #16* 13]
    stp x28, x29, [sp, #16* 14]
   	add x21, sp, #S_FRAME_SIZE 
   	mrs x22, elr_el1
   	mrs x23, spsr_el1

   	stp x30, x21, [sp, #16* 15]
	stp x22, x23, [sp, #16* 16]		
	ret

kernel_exit:
   	ldp x21, x22, [sp, #16* 16]

   	msr elr_el1, x21
   	msr spsr_el1, x22

   	ldr x30, [sp, #16* 15]

   	ldp x0, x1, [sp, #16* 0]
   	ldp x2, x3, [sp, #16* 1]
   	ldp x4, x5, [sp, #16* 2]
   	ldp x6, x7, [sp, #16* 3]
   	ldp x8, x9, [sp, #16* 4]
   	ldp x10, x11, [sp, #16* 5]
   	ldp x12, x13, [sp, #16* 6]
   	ldp x14, x15, [sp, #16* 7]
   	ldp x16, x17, [sp, #16* 8]
   	ldp x18, x19, [sp, #16* 9]
   	ldp x20, x21, [sp, #16* 10]
   	ldp x22, x23, [sp, #16* 11]
   	ldp x24, x25, [sp, #16* 12]
   	ldp x26, x27, [sp, #16* 13]
   	ldp x28, x29, [sp, #16* 14]
   	add sp, sp, #S_FRAME_SIZE 
   	eret


.global string_test
string_test:
	.string "t"

.global trigger_alignment
trigger_alignment:
	ldr x0, =0x80002
	ldr x1, [x0]
	ret

