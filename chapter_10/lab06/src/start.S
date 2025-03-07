.section ".text.boot"

.global _start

_start:
    mrs x1, MPIDR_EL1 
    and x1,x1,#3
    cbz x1, 2f

1:  wfe
    b 1b 

2:  adr x0, TEXT_ROM
    adr x1, _text_start
    adr x2, _text_end    
    
3:  ldr x3, [x0], #8
    str x3, [x1], #8
    cmp x1, x2
    b.cc 3b

    ldr x1, =_start
    mov sp,x1

    ldr x1, =_bss_start 
    ldr x2, =_bss_end
4:  cmp x1,x2   
    b.eq 5f
    str xzr, [x1], #8
    b 4b

5:  
    bl main 
    b 1b
