.align 3
.section ".text"

.global main
main:
	stp x29, x30, [sp,#-16]!

	//a:10, b:100
	mov x0, 10
	mov x1, 100
	stp x0, x1, [sp,#-16]!
	bl get_max

	mov x2, x0
	ldp x0, x1, [sp]
	bl print_ret

	ldp x0, x1, [sp], #16

	//a:902, b:100
	mov x0, 902
	stp x0, x1, [sp,#-16]!
	bl get_max

	mov x2, x0
	ldp x0, x1, [sp]
	bl print_ret

	ldp x0, x1, [sp], #16

	//a:-102, b:100
	mov x0, -102
	stp x0, x1, [sp,#-16]!
	bl get_max

	mov x2, x0
	ldp x0, x1, [sp]
	bl print_ret

	ldp x0, x1, [sp], #16

	//a:-102, b:-10
	mov x1, -10
	stp x0, x1, [sp,#-16]!
	bl get_max

	mov x2, x0
	ldp x0, x1, [sp], #16
	bl print_ret
	
	mov x0, 0

	ldp x29, x30, [sp], #16
	ret	

//x0:a, x1:b, x2:max
print_ret:
	stp x29, x30, [sp,#-16]!

	mov x3, x2
	mov x2, x1
	mov x1, x0

	adrp x0, str_max_num_fmt
	add x0, x0, #:lo12:str_max_num_fmt 

	bl printf

	ldp x29, x30, [sp], #16
	ret	

.align 3
.section ".rodata"
str_max_num_fmt:	
	.string	"a:%lld,b:%lld,max:%lld\r\n"

