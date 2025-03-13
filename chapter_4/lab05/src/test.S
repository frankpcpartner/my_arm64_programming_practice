.section ".text"

.align 3
.global _test
_test:
    stp x29, x30, [sp,#-16]!

    mrs x1, id_aa64isar0_el1

    ubfx x0, x1, 20, 4
    ubfx x2, x1, 4, 4

    cbnz x0, 1f
    ldr x0, =str_not_support_lse_fmt
    bl printf
    b 2f    

1:  ldr x0, =str_support_lse_fmt
    bl printf

2:  cbnz x2, 3f
    ldr x0, =str_not_support_aes_fmt
    bl printf
    b 4f

3:  ldr x0, =str_support_aes_fmt    
    bl printf

4:
    ldp x29, x30, [sp], #16
    ret

.section ".rodata"
.align 3
str_not_support_lse_fmt:    
    .string "\rNot supoort LSE\n"

.align 3
str_support_lse_fmt:    
    .string "\rSupoort LSE\n"

.align 3
str_not_support_aes_fmt:    
    .string "\rNot supoort AES\n"

.align 3
str_support_aes_fmt:    
    .string "\rSupoort AES\n"
