.section ".text"

.global _test
.align 3
_test:
    mov x1, #0x80000
    mov x3, #16
    ldr x2, [x1]
    ldr x2, [x1, #8]
    ldr x2, [x1, x3]
    ldr x2, [x1, x3, LSL #3]
    ret
