.section ".text"

#define MY_LABEL 0x20
.align 3
.global _test

_test:
    ldr x6, MY_LABEL
    //x6 is the data stored in pc + 0x20
    ldr x7, =MY_LABEL
    //x7 is 0x20
    ret 
