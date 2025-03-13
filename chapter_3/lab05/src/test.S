.section ".text"

#define MY_LABEL 0x20
.align 3
.global _test

_test:
    mov x1, 0x80000
    add x2, x1, #32
    mov x3, 0x200000

1:  cmp x1,x2
    beq 2f
    ldp x4, x5, [x1], #16   
    stp x4, x5, [x3], #16
    b 1b
2:
    ret 
