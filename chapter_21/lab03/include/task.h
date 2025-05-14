#ifndef _TASK_H
#define _TASK_H

#include "page.h"
#include "list.h"

#ifndef _BUILD_ASM_OFFSET_
#include "asm-offsets.h"
#endif

#define THREAD_SIZE (1 * PAGE_SIZE)
#define THREAD_START_SP (THREAD_SIZE - 8)

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
    list_head_t run_list;
	enum task_state state;
	enum task_flags flags;
	long long counter;
    int priority;
    int need_resched;
    int preempt_count;
    int pid;
    cpu_context_t cpu_context;
    struct task_struct_s* next_task;   
    struct task_struct_s* prev_task;   
}task_struct_t;


#define INIT_TASK(task)   \
{                         \
    .state = TASK_RUNNING,\
    .priority = 20,        \
    .counter = DEFAULT_COUNTER, \
    .flags = PF_KTHREAD,  \
    .pid = 0,             \
    .preempt_count = 0, \
    .need_resched = 0, \
    .next_task = (task_struct_t *)&task, \
    .prev_task = (task_struct_t *)&task, \
}

union task_union
{
    task_struct_t task;
    unsigned long long stack[THREAD_SIZE/sizeof(long long)];
};

extern task_struct_t* g_task[MAX_TASK];
extern union task_union init_task_union;

#define __init_task_data __attribute__((__section__(".data.init_task")))

#define init_task (init_task_union.task)

#define SET_LINKS(p) do { \
	(p)->next_task = &init_task; \
	(p)->prev_task = init_task.prev_task; \
	init_task.prev_task->next_task = (p); \
	init_task.prev_task = (p); \
	} while (0)

#define for_each_task(p) \
    for (p = &init_task; (p = p->next_task) != &init_task; )

register unsigned long long current_stack_pointer asm ("sp");

static inline task_struct_t* get_current()
{

    return (task_struct_t*)
        (current_stack_pointer & ~(THREAD_SIZE -1));
}

#define current get_current()

static inline void clear_task_resched(task_struct_t *p)
{
	p->need_resched = 0;
}

#define PREEMPT_BITS	8
#define HARDIRQ_BITS	4

#define PREEMPT_SHIFT	0
#define HARDIRQ_SHIFT	(PREEMPT_SHIFT + PREEMPT_BITS)

#define PREEMPT_OFFSET	(1UL << PREEMPT_SHIFT)
#define HARDIRQ_OFFSET	(1UL << HARDIRQ_SHIFT)

#define __IRQ_MASK(x)	((1UL << (x))-1)
#define PREEMPT_MASK	(__IRQ_MASK(PREEMPT_BITS) << PREEMPT_SHIFT)
#define HARDIRQ_MASK	(__IRQ_MASK(HARDIRQ_BITS) << HARDIRQ_SHIFT)

#define preempt_count() (current->preempt_count)

#define hardirq_count()	(preepmt_count() & HARDIRQ_MASK)

#define in_atomic_preempt_off() \
	(preempt_count() != 1)

static void preempt_enable()
{
	current->preempt_count --;
}

static void preempt_disable()
{
	current->preempt_count ++;
}

static inline void preempt_add(int val)
{
	current->preempt_count += val;
}

static inline void preempt_sub(int val)
{
	current->preempt_count -= val;
}

#define __irq_enter() preempt_add(HARDIRQ_OFFSET)
#define __irq_exit() preempt_sub(HARDIRQ_OFFSET)

int do_fork(unsigned long long clone_flags,
	unsigned long long fn,
	unsigned long long arg);

void preempt_schedule_irq();

task_struct_t* switch_to(task_struct_t*prev, task_struct_t* next);

void task_handle_timer();

extern task_struct_t * cpu_switch_to(task_struct_t *prev, task_struct_t *next);
extern void ret_from_fork(void);

void sched_init();

void schedule_tail(task_struct_t* prev);

#endif

#endif