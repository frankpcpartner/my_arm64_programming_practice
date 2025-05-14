#include "core.h"
#include "task.h"
#include "util.h"
#include "exception.h"

union task_union init_task_union __init_task_data = {INIT_TASK(task)};

task_struct_t* g_task[MAX_TASK] = {&init_task_union.task,};

task_struct_t *current = &init_task_union.task;

void cpu_switch_to(task_struct_t *prev, task_struct_t *next);
void ret_from_fork();

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


int do_fork(unsigned long long clone_flags,
	unsigned long long fn,
	unsigned long long arg)
{
	task_struct_t* p = NULL;
    int pid = -1;   
    
    p = (task_struct_t*) get_free_page();
    if(!p)
    	goto error;
    

    pid = find_empty_task();
    if(pid < 0)
    	goto error;
    
    if(copy_thread(clone_flags, p, fn, arg))
    	goto error;

    p->state = TASK_RUNNING;
    p->pid = pid;
    g_task[pid] = p;

    return pid;
error:
	if(p)
	{
		free_page((unsigned long long)p);
		p = NULL;
	}
	return -1;
}

void switch_to(task_struct_t* next)
{
    task_struct_t* prev = current;
    
    if(current == next)
    	return;

    current = next;
    cpu_switch_to(prev,next);
}

