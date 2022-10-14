#include "param.h"

#ifndef QUEUE
#define QUEUE

struct queue
{
    struct proc *array[NPROC];
    int head;
    int tail;
    int maxticks;
};

#endif