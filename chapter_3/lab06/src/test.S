.section ".text"

#define MY_LABEL 0x20
.align 3
.global _test

//x0:The dest address, x1:The input value, x2: count
_test:
    cmp x2, xzr 
    beq 4f    

    //Make x7="x1,x1,x1,x1,x1,x1,x1,x1" 
    mov x3, x1
    orr x7, x3, x3, lsl #8
    orr x7, x7, x7, lsl #16
    orr x7, x7, x7, lsl #32
    
    //Check if 16-byte aligment
    mov x3, #0x0f
    and x4, x0, x3
    and x5, x2, x3
    orr x6, x4, x5
    cmp x6, xzr
    beq 2f

1:  //Not 16 byte aligment
    strb w1, [x0]
    add x0, x0, #1
    sub x2, x2, #1
    
    cmp x2, xzr 
    beq 4f 

    and x5, x2, x3
    cmp x5, xzr
    beq 2f
    b 1b

2:  // 16 byte aligment
    //x8 end address
    add x8, x0, x2

3:  cmp x0,x8
    beq 4f    
    stp x7, x7, [x0], #16  
    b 3b
4:
    ret
