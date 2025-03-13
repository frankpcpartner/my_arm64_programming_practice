.section ".text"

.align 3
.global _test

_test:
    stp x29, x30, [sp, #-16]!

    mov x1, 1
    mov x2, -3

1:  cmn x1, x2
    add x2, x2 , 1
    mrs x3, nzcv
    b.mi 1b

    mov x2, 3
2:  cmp x2,x1
    add x1, x1, 1
    mrs x3, nzcv
    b.ge 2b
    
    ldp x29, x30, [sp], #16
    ret

.section ".rodata"
