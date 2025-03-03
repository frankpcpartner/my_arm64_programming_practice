.section ".text.boot"

.global _start

_start:
    mrs x1, MPIDR_EL1 
    and x1,x1,#3
    cbz x1, 2f

1:  wfe
    b 1b 

2:  adrp x1, _start
    add x1, x1, #:lo12:_start
    mov sp,x1

    adrp x1, _bss_start
    add x1, x1, #:lo12:_bss_start

    adrp x2, _bss_end
    add x2, x2, #:lo12:_bss_end

3:  cmp x1,x2   
    b.eq 4f
    str xzr, [x1], #8
    b 3b

4:  ldr x0, =_start
    adrp x1, _start
    add x1, x1, #:lo12:_start
    cmp x0, x1
    b.eq 5f

    sub x0, x1, x0
    adrp x1, _rel_dyn_start
    add x1, x1,  #:lo12:_rel_dyn_start   
    bl relocate_code
    
5:  bl main 
    b 1b
