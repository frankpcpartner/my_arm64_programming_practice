#ifndef _SCHED_H
#define _SCHED_H

#include "core.h"
#include "list.h"
#include "timer.h"
#include "task.h"

#define DEFAULT_COUNTER (HZ / 10)

typedef struct run_queue_s 
{
	list_head_t rq_head;
	unsigned int nr_running;
    unsigned long long nr_switches;
    task_struct_t* curr;
}run_queue_t;

typedef struct sched_class_s 
{
	const struct sched_class_s* next;

    void (*task_fork)(task_struct_t* p);
    void (*enqueue_task)(run_queue_t* rq, task_struct_t* p);
    void (*dequeue_task)(run_queue_t* rq, task_struct_t* p);
    void (*task_tick)(run_queue_t* rq, task_struct_t* p);
    task_struct_t* (*pick_next_task)(run_queue_t* rq, task_struct_t* prev);

}sched_class_t;

void task_fork(task_struct_t* p);
void enqueue_task(run_queue_t* rq, task_struct_t* p);
void dequeue_task(run_queue_t* rq, task_struct_t* p);
void task_tick(run_queue_t* rq, task_struct_t* p);
task_struct_t* pick_next_task(run_queue_t* rq, task_struct_t* prev);

#endif