.align 3
.section ".data"

.global my_data
my_data:
	.dword 0x0



.align 3
.section ".text"

.global my_atomic_write
.global atomic_set

my_atomic_write:
	adr x6, my_data

1:
	ldxr x2, [x6]
	mov  x2, x0
	stxr w3, x2, [x6]
	cbnz w3, 1b
	
	mov x0, x2
	ret	

atomic_set:

1:
	ldxr x2, [x1]
	mov x4, #1
	lsl x4, x4, x0
	orr x2, x2, x4  
	stxr w3, x2, [x1]
	cbnz w3, 1b

	ret