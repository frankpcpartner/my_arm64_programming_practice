.section ".text"

.align 3
.global _test

_test:
    stp x29, x30, [sp, #-16]!

    adr x0, my_test_data
    ldr x1, [x0]

    adrp x0, my_test_data 
    add x0, x0, #:lo12:my_test_data    
    ldr x1, [x0]

    adrp x0, init_pg_dir
    add x0, x0, #:lo12:init_pg_dir

    ldr x1, =init_pg_dir
    

    ldp x29, x30, [sp], #16
    ret

.section ".rodata"
.align 3
.global my_test_data
my_test_data:
    .dword 0x12345678abcdabcd
