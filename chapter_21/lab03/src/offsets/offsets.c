#include <stdio.h>
#include <stddef.h>

#include "offsets.h"
#include "exception.h"
#include "task.h"


int main(void) {
    

    printf("#ifndef _ASM_OFFSETS_H\r\n");
    printf("#define _ASM_OFFSETS_H\r\n");
    printf("/*\r\n");
    printf( " * DO NOT MODIFY.\r\n");
    printf( " *\r\n");
    printf( " * This file was generated automatically.\r\n");
    printf( " */\r\n"); 
    printf( "\r\n");
    DEFINE(S_FRAME_SIZE, sizeof(pt_regs_t));
    DEFINE(S_X0,    offsetof(pt_regs_t, regs[0]));
    DEFINE(S_X1,    offsetof(pt_regs_t, regs[1]));
    DEFINE(S_X2,    offsetof(pt_regs_t, regs[2]));
    DEFINE(S_X3,    offsetof(pt_regs_t, regs[3]));
    DEFINE(S_X4,    offsetof(pt_regs_t, regs[4]));
    DEFINE(S_X5,    offsetof(pt_regs_t, regs[5]));
    DEFINE(S_X6,    offsetof(pt_regs_t, regs[6]));
    DEFINE(S_X7,    offsetof(pt_regs_t, regs[7]));
    DEFINE(S_X8,    offsetof(pt_regs_t, regs[8]));
    DEFINE(S_X10,   offsetof(pt_regs_t, regs[10]));
    DEFINE(S_X12,   offsetof(pt_regs_t, regs[12]));
    DEFINE(S_X14,   offsetof(pt_regs_t, regs[14]));
    DEFINE(S_X16,   offsetof(pt_regs_t, regs[16]));
    DEFINE(S_X18,   offsetof(pt_regs_t, regs[18]));
    DEFINE(S_X20,   offsetof(pt_regs_t, regs[20]));
    DEFINE(S_X22,   offsetof(pt_regs_t, regs[22]));
    DEFINE(S_X24,   offsetof(pt_regs_t, regs[24]));
    DEFINE(S_X26,   offsetof(pt_regs_t, regs[26]));
    DEFINE(S_X28,   offsetof(pt_regs_t, regs[28]));
    DEFINE(S_FP,    offsetof(pt_regs_t, regs[29]));
    DEFINE(S_LR,    offsetof(pt_regs_t, regs[30]));
    DEFINE(S_SP,    offsetof(pt_regs_t, sp));
    DEFINE(S_PSTATE, offsetof(pt_regs_t, pstate));
    DEFINE(S_PC,     offsetof(pt_regs_t, pc));
    DEFINE(S_ORIG_X0, offsetof(pt_regs_t, orig_x0));
    DEFINE(S_SYSCALLNO, offsetof(pt_regs_t, syscallno));
    DEFINE(S_ORIG_ADDR_LIMIT, offsetof(pt_regs_t, orig_addr_limit));
    DEFINE(S_STACKFRAME, offsetof(pt_regs_t, stackframe));

    DEFINE(THREAD_CPU_CONTEXT, offsetof(task_struct_t, cpu_context));
    DEFINE(TI_PREEMPT,  offsetof(task_struct_t, preempt_count));
    DEFINE(NEED_RESCHED, offsetof(task_struct_t, need_resched));

    printf( "\r\n");
    printf( "#endif\r\n");
    return 0;    
}