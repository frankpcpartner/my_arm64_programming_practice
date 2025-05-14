.section ".text"
.align 3
.global get_cache_line_size
get_cache_line_size:
	mrs x1, ctr_el0
	ubfm x1, x1, #16, #19
	mov x0, #4
	lsl x0, x0, x1
	ret

//x29, x30, start_addr, end_addr
//  0,   8,         16,       24
.global flush_cache_range
flush_cache_range:
	stp x29, x30, [sp, #-32]!
	mov x29, sp

	str x0, [sp, #16]
	str x1, [sp, #24]

   bl get_cache_line_size
   sub x1, x0, #1
   ldr x2, [sp, #16]
   bic x2, x2, x1

   ldr x3, [sp, #24]
1:
   dc civac, x2
   add x2, x2, x0
   cmp x2, x3
   b.lo 1b    

   dsb	ish

	ldp x29, x30, [sp], #32
	ret