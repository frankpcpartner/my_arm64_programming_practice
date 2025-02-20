.section ".text.boot"

.global _start

_start:
    mrs x1, MPIDR_EL1 
    and x1,x1,#3
    cbz x1, 2f

1:  wfe
    b 1b 

2:  ldr x1, =_start
    mov sp,x1

    ldr x1, =_bss_start 
    ldr x2, =_bss_end
3:  cmp x1,x2   
    b.eq 4f
    str xzr, [x1], #8
    b 3b

4:  
    bl _test
    bl main 
    b 1b
