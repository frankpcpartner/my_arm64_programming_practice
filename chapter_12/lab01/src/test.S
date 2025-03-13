.section ".text"

.macro add_m a, b, label
    mov x0, \a
    mov x1, \b
    bl add_\()\label\()
.endm

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
    
    mov x3, 10
    mov x4, 15 
    add_m x3, x4, 1

    mov x3, 15
    mov x4, -5
    add_m x3, x4, 2

    ldp x29, x30, [sp], #16
    ret

add_1:
    stp x29, x30, [sp, #-16]!
    add sp, sp, #-16

    add x0, x0, x1
    
    str x0, [sp, #8]

    adrp x0, str_in_add_1_fmt
    add x0, x0, #:lo12:str_in_add_1_fmt 

    bl printf

    ldr x0, [sp, #8]
    add sp, sp, #16
    ldp x29, x30, [sp], #16
    ret    


add_2:
    stp x29, x30, [sp, #-16]!
    add sp, sp, #-16

    add x0, x0, x1
    
    str x0, [sp, #8]    

    adrp x0, str_in_add_2_fmt
    add x0, x0, #:lo12:str_in_add_2_fmt 

    bl printf

    ldr x0, [sp, #8]
    add sp, sp, #16
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

.align 3
str_in_add_1_fmt:
    .string "\rIn add_1\n"

.align 3
str_in_add_2_fmt:
    .string "\rIn add_2\n"

