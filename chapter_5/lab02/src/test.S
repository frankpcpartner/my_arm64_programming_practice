.section ".text"

.align 3
.global _test

_test:
    stp x29, x30, [sp, #-16]!

    cmp x0, 0 
    add x2, x1, 2
    add x3, x1, -1
    csel x0, x2, x3, eq


    ldp x29, x30, [sp], #16
    ret

.section ".rodata"
