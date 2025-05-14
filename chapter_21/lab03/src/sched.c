#include "sched.h"

extern const sched_class_t simple_sched_class;


void task_fork(task_struct_t* p)
{
	simple_sched_class.task_fork(p);
}

void enqueue_task(run_queue_t* rq, task_struct_t* p)
{
	simple_sched_class.enqueue_task(rq, p);
}

void dequeue_task(run_queue_t* rq, task_struct_t* p)
{
	simple_sched_class.dequeue_task(rq, p);
}

void task_tick(run_queue_t* rq, task_struct_t* p)
{
	simple_sched_class.task_tick(rq, p);
}

task_struct_t* pick_next_task(run_queue_t* rq, task_struct_t* prev)
{
	return simple_sched_class.pick_next_task(rq, prev);
}
