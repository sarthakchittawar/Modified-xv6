#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "riscv.h"
#include "spinlock.h"
#include "proc.h"
#include "defs.h"
#include "queue.h"

// returns size of queue
int enqueue(struct queue *q, struct proc *p)
{
    if (q == 0)
        panic("Invalid queue");
    if (p == 0)
        panic("Inavlid proc");
    int index = q->tail;
    if (index >= NPROC)
        panic("Queue full");
    
    q->array[index] = p;
    p->queued = 1;
    p->timeslice = 0;
    q->tail++;

    return q->tail;
}

// returns proc * which was removed
struct proc *dequeue(struct queue *q, int index)
{
    if (q == 0)
        panic("Invalid queue");
    if (index < 0 || index > q->tail)
        panic("Wrong index to dequeue");
    
    struct proc *p = 0;
    if (q!=0)
    {
        for(int i = index + 1; i< q->tail; i++)
        {
            q->array[i - 1] = q->array[i];
        }
        p = q->array[index];
        q->tail--;
        p->queued = 0;
    }
    return p;
}

// returns new size of queue
int rmproc(struct queue *q, struct proc *p)
{
    int index = -1;
    if (q == 0)
        panic("Invalid queue");
    if (p == 0)
        panic("Inavlid proc");

    for(int i=0; i<q->tail; i++)
    {
        if (q->array[i] == p)
        {
            index = i;
            break;
        }
    }
    if (index == -1)
        panic("No such process in queue");
    for(int i= index + 1; i<q->tail; i++)
    {
        q->array[i - 1] = q->array[i];
    }
    q->tail--;
    p->queued = 0;

    return q->tail;
}