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
	ventry	irq_invalid_el1
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


.global string_test
string_test:
	.string "t"

.global trigger_alignment
trigger_alignment:
	ldr x0, =0x80002
	ldr x1, [x0]
	ret

