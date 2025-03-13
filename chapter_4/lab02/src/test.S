.section ".text"

.align 3
.global _test
//X0 a, X1 b
_test:
    stp x29, x30, [sp,#-16]!

    cmp x0, x1
    sbc x0, xzr, xzr
      
    ldp x29, x30, [sp], #16
    ret

