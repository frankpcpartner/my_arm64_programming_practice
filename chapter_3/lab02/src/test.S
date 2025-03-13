.section ".text"

.global _test
.align 3
_test:
    mov x1, #0x80000
    mov x3, #16
    ldr x6, [x1,#8]!
    //x1 0x80008, x6 is the data stored at address 0x80008.
    ldr x7, [x1], #8
    //x1 0x80010, x7 is the data stored at address 0x80008.
    ret
