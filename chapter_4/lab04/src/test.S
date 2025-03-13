.section ".text"

.align 3
.global _test
_test:
    stp x29, x30, [sp,#-16]!

    mov x1, 0x345
    bfi x0, x1, #8, #4
    
    mov x1, x0
    ldr x0, =str_bfi_ret_fmt
    bl printf

    ldr x2, =0x5678abcd
    ubfx x3, x2, 4, 8
    sbfx x4, x2, 4, 8

    ldr x0, =str_bfx_sbfx_ret_fmt
    mov x1, x3
    mov x2, x4
    bl printf

    ldp x29, x30, [sp], #16
    ret

.section ".rodata"
.align 3
str_bfi_ret_fmt:
    .string "\bfi ret is 0x%x\n"

.align 3
str_bfx_sbfx_ret_fmt:
    .string "\bfx ret is 0x%x,sbfx ret is 0x%x\n"
