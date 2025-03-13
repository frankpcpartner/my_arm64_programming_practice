.section ".text"

.global _test1
.global _test2
.align 3

_test1:
    stp x29, x30, [sp, #-16]!
    
    mov x3, 0
    mov x0, 0xffffffffffffffff
    mov x1, 1
    adds x0, x0, x1
    adc x3, x3, xzr
    cbz x3, 1f
    
    //x3 == 1
    ldr x0, =str_c_is_set
    bl printf
    b 2f
1:
    //x3 == 0
    ldr x0, =str_c_is_not_set
    bl printf    
2:
    ldp x29, x30, [sp], #16
    ret

_test2:
    stp x29, x30, [sp, #-16]!
    
    /*when x1 > x2，C=1
      when x1 < x2，C=0
    */

    mov x1, 0x1
    mov x2, 0x3
    cmp x1, x2
    adc x3, xzr, xzr

    cbz x3, 1f

    //x3 == 1
    ldr x0, =str_c_is_set
    bl printf
    b 2f
1:
    //x3 == 0
    ldr x0, =str_c_is_not_set
    bl printf    
2:
    ldp x29, x30, [sp], #16
    ret


.section ".rodata"
.align 3
str_c_is_set:
    .string "\rThe c is set.\n"
.align 3
str_c_is_not_set:
    .string "\rThe c is not set.\n"
