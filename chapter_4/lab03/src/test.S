.section ".text"

.align 3
.global _test
_test:
    stp x29, x30, [sp,#-16]!

    mov x2, 1
    ands x3, x2, 2   

    mrs x1, nzcv
    ldr x0, =str_fmt
    bl printf

    ands x3, x2, 0xff    
    
    mrs x1, nzcv
    ldr x0, =str_fmt
    bl printf

    ldp x29, x30, [sp], #16
    ret

.section ".rodata"
.align 3
str_fmt:
    .string "\rnzcv is 0x%x\n"
