#include "core.h"
#include "task.h"
#include "util.h"
#include "exception.h"
#include "sched.h"
#include "irq.h"

union task_union init_task_union __init_task_data = {INIT_TASK(init_task_union.task)};

task_struct_t* g_task[MAX_TASK] = {&init_task_union.task,};

static run_queue_t g_rq;


/*
 * pt_regs存储在栈顶
 */
static pt_regs_t *task_pt_regs(task_struct_t *tsk)
{
	unsigned long long p;

	p = (unsigned long long)tsk + THREAD_SIZE - sizeof(pt_regs_t);

	return (pt_regs_t *)p;
}

static int find_empty_task()
{
	for(int i = 0; i < MAX_TASK; i++)
	{
		if(g_task[i] == NULL)
			return i;
	}

	return -1;
}

static int copy_thread(
	unsigned long long clone_flags, 
	task_struct_t* p, 
	unsigned long long fn,
	unsigned long long arg)
{
	pt_regs_t* childregs;

	childregs = task_pt_regs(p);
	memset(childregs, 0, sizeof(pt_regs_t));
	memset(&p->cpu_context,0,sizeof(cpu_context_t));

    if(clone_flags & PF_KTHREAD)
    {
    	childregs->pstate = PSR_MODE_EL1h;
    	p->cpu_context.x19 = fn;
    	p->cpu_context.x20 = arg;
    }

    p->cpu_context.pc = (unsigned long long) ret_from_fork;
    p->cpu_context.sp = (unsigned long long) childregs;

    return 0;
}

static void wake_up_process(task_struct_t *p)
{
	run_queue_t *rq = &g_rq;

	p->state = TASK_RUNNING;

	enqueue_task(rq, p);
}

int do_fork(unsigned long long clone_flags,
	unsigned long long fn,
	unsigned long long arg)
{
	task_struct_t* p = NULL;
    int pid = -1;   
    
    p = (task_struct_t*) get_free_page();
    if(!p)
    	goto error;
    
    memset(p, 0, sizeof(task_struct_t));

    pid = find_empty_task();
    if(pid < 0)
    	goto error;
    
    if(copy_thread(clone_flags, p, fn, arg))
    	goto error;

    p->state = TASK_RUNNING;
    p->pid = pid;
    p->counter = (current->counter + 1) >> 1;
    current->counter >>= 1;
    p->need_resched = 0;
    p->priority = 2;
    SET_LINKS(p);
    g_task[pid] = p;
    
    wake_up_process(p);

    return pid;
error:
	if(p)
	{
		free_page((unsigned long long)p);
		p = NULL;
	}
	return -1;
}

task_struct_t* switch_to(task_struct_t*prev , task_struct_t* next)
{
    if(prev == next)
    	return;

    return cpu_switch_to(prev,next);
}

void schedule_tail(task_struct_t* prev)
{
	raw_local_irq_enable();
}

static void schedule_debug(task_struct_t *p)
{
	if (in_atomic_preempt_off())
		printf("BUG: scheduling while atomic: %d, 0x%x\n",
				p->pid, preempt_count());
}

static void __schedule()
{
    task_struct_t* prev, *next, *last;
    run_queue_t* rq = &g_rq;
    
    prev = current;
    
    schedule_debug(prev);

    raw_local_irq_disable();

    if(prev->state)
    	dequeue_task(rq, prev);

    next = pick_next_task(rq, prev);
    clear_task_resched(prev);

    if(next != prev)
    {
        last = switch_to(prev, next);
        rq->nr_switches ++;
        rq->curr = current;
    }

    schedule_tail(last);
}

void schedule()
{
    preempt_disable();
    __schedule();
    preempt_enable();
}

void preempt_schedule_irq()
{
    if(preempt_count())
        printf("\rBUG: %s incorrect preempt count: 0x%x\n",
            __func__, preempt_count());

    preempt_disable();

    raw_local_irq_enable();
    __schedule();
    raw_local_irq_disable();

    preempt_enable();
}

void task_handle_timer()
{
    run_queue_t* rq = &g_rq;
    task_tick(rq, current);    
}

void sched_init(void)
{
	run_queue_t *rq = &g_rq;

	INIT_LIST_HEAD(&rq->rq_head);
	rq->nr_running = 0;
	rq->nr_switches = 0;
	rq->curr = NULL;
}
