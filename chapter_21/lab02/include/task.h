#ifndef _TASK_H
#define _TASK_H

#include "page.h"

#define THREAD_SIZE (1 * PAGE_SIZE)
#define THREAD_START_SP (THREAD_SIZE - 8)
#define THREAD_CPU_CONTEXT 24 /* offsetof(struct task_struct, cpu_context)  // */

#define MAX_TASK 16

/*
 * PSR bits
 */
#define PSR_MODE_EL0t   0x00000000
#define PSR_MODE_EL1t   0x00000004
#define PSR_MODE_EL1h   0x00000005
#define PSR_MODE_EL2t   0x00000008
#define PSR_MODE_EL2h   0x00000009
#define PSR_MODE_EL3t   0x0000000c
#define PSR_MODE_EL3h   0x0000000d
#define PSR_MODE_MASK   0x0000000f

#ifndef _ASM_

enum task_state 
{
    TASK_RUNNING = 0,
    TASK_INTERRUPTIBLE = 1,
    TASK_UNINTERRUPTIBLE = 2,
    TASK_ZOMBIE = 3,
    TASK_STOPPED = 4,
};

enum task_flags
{
    PF_KTHREAD = 1 << 0,
};

typedef struct cpu_context_s
{
    unsigned long long x19;
    unsigned long long x20;
    unsigned long long x21;
    unsigned long long x22;
    unsigned long long x23;
    unsigned long long x24;
    unsigned long long x25;
    unsigned long long x26;
    unsigned long long x27;
    unsigned long long x28;
    unsigned long long fp;
    unsigned long long sp;
    unsigned long long pc;
}cpu_context_t;

typedef struct task_struct_s 
{
	enum task_state state;
	enum task_flags flags;
	long long count;
    int priority;
    int pid;
    cpu_context_t cpu_context;
}task_struct_t;


#define INIT_TASK(task)   \
{                         \
    .state = TASK_RUNNING,\
    .priority = 1,        \
    .flags = PF_KTHREAD,  \
    .pid = 0,             \
}

union task_union
{
    task_struct_t task;
    unsigned long long stack[THREAD_SIZE/sizeof(long long)];
};

extern task_struct_t* g_task[MAX_TASK];

#define __init_task_data __attribute__((__section__(".data.init_task")))

int do_fork(unsigned long long clone_flags,
	unsigned long long fn,
	unsigned long long arg);

#endif

#endif