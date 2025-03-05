.align 3
.section ".text"

.global main
main:
	stp x29, x30, [sp,#-16]!

	ldr x0, =num
	ldr x1, num_count
 
	//Set x2 as long long min
	mov x2, xzr 	//Clear W0 to 0
	movn x2, #0 	//x0 = 0xFFFFFFFFFFFFFFFF (-1)
	lsl x2, x2, #63 // Shift left to get 0x8000000000000000 (-2^63)	

1:	cbz x1, 2f
	ldr x3, [x0], #8		
	sub x1, x1, 1	

	cmp x3, x2
	ble 1b  
	mov x2, x3
	b 1b
2:
	adrp x0,str_max_num_fmt
	add x0, x0, #:lo12:str_max_num_fmt
	mov x1, x2
	bl printf

	mov x0, 0

	ldp x29, x30, [sp], #16
	ret	


.align 3
.section ".rodata"
num:
	.quad  1
	.quad  2
	.quad  -5
	.quad  11
	.quad  -125
	.quad  55

num_count:
	.quad  6

.align 3
str_max_num_fmt:
	.string "\r Max num is %d.\n"