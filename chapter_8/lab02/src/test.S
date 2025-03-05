.align 3
.section ".text"

.global get_max
get_max:
	stp x29, x30, [sp,#-16]!

	cmp x0, x1
	bgt 1f
	mov x0, x1

1:
	ldp x29, x30, [sp], #16
	ret	


