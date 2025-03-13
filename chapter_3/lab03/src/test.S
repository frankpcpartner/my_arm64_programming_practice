.section ".text"

.global _test
.align 3
_test:
    mov x2, 0x400000
    ldr x6, =0x1234abce
    str x6, [x2, #8]!
    //x2 is 0x400008, The data in 0x400000 is 0x0 
    mov x2, 0x500000
    str x6, [x2], #8
    //x2 is 0x500008, The data in 0x500000 is 0xce
    ret
