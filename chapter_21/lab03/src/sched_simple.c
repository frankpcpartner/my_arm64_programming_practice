#include "sched.h"
#include "util.h"
#include "list.h"

static void enqueue_task_simple(run_queue_t* rq, task_struct_t* p);
static void dequeue_task_simple(run_queue_t* rq, task_struct_t* p);
static void task_tick_simple(run_queue_t* rq, task_struct_t* p);
static task_struct_t* pick_next_task_simple(run_queue_t* rq, task_struct_t* prev);

const sched_class_t simple_sched_class = 
{
	.next = NULL,
    .dequeue_task = dequeue_task_simple,
    .enqueue_task = enqueue_task_simple,
    .task_tick = task_tick_simple,
    .pick_next_task = pick_next_task_simple,
};

static void enqueue_task_simple(run_queue_t* rq, task_struct_t* p)
{
    list_add(&p->run_list, &rq->rq_head);
    rq->nr_running ++;
}

static void dequeue_task_simple(run_queue_t* rq, task_struct_t* p)
{
	rq->nr_running --; 
	list_del(&p->run_list);
}

static void task_tick_simple(run_queue_t* rq, task_struct_t* p)
{
    if( --p->counter <= 0)
    {
    	p->counter = 0;
    	p->need_resched = 1;
    	printf("\rpid:%d need_resched\n", p->pid);
    }
}

static int goodness(task_struct_t *p)
{
    int weight;

    weight = p->counter;

    return weight;
}

static void reset_score(void)
{
    task_struct_t *p;

    for_each_task(p) 
    {
        p->counter = DEFAULT_COUNTER + p->priority;    
    }
}

static task_struct_t* pick_next_task_simple(run_queue_t* rq, task_struct_t* prev)
{
	task_struct_t* p, *next;
	list_head_t* tmp;
	int weight;
	int c;

repeat:
    c = -1000;
    list_for_each(tmp, &rq->rq_head) 
    {
    	p = list_entry(tmp, task_struct_t, run_list);
        weight = goodness(p);
        
        if(weight > c)
        {
        	c = weight;
        	next = p;
        }
    }


    if(!c)
    {
    	reset_score();
    	goto repeat;
    }	

    return next;
}


