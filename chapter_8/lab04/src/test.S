.section ".text"

.align 3
.global _test

_test:
    stp x29, x30, [sp, #-16]!

    adr x0, my_test_data
    ldr x1, [x0]

    adrp x0, my_test_data 
    add x0, x0, #:lo12:my_test_data    
    ldr x1, [x0]

    adrp x0, init_pg_dir
    add x0, x0, #:lo12:init_pg_dir

    ldr x1, =init_pg_dir
    

    ldp x29, x30, [sp], #16
    ret

.section ".rodata"
.global my_test_data
.global fun_addr
.global fun_name
.global fun_num

.align 3
my_test_data:
    .dword 0x12345678abcdabcd

.align 3
fun_addr:
    .quad 0x800800
    .quad 0x800860
    .quad 0x800880

.align 3
fun_name:
    .asciz "func_a"
    .asciz "func_b"
    .asciz "func_c"

.align 3
fun_num:
    .word   3

