// (c) Partha Dasgupta 2009
// permission to use and distribute granted.

#include "threads.h"

// Semaphore structure which has a Semaphore Queue with a head pointer and a count
typedef struct Semaphore_t{
     TCB_t **SemQ;
     TCB_t *s_head;
     int count;  //Counter
} Semaphore_t;

// Blocks the current thread by adding it to the Semaphore Queue and swaps context to the next thread in the ReadyQ
void block_tcb(Semaphore_t *sem){
    TCB_t *Prev_Thread;
    AddQueue(sem->SemQ, Curr_Thread);  // Adds the current thread to the readyQ
    Prev_Thread = Curr_Thread;      // Stores address of current thread as previous thread
    Curr_Thread = DelQueue(ReadyQ); // Selects the next thread to run by deleting a TCB from the queue
    swapcontext(&(Prev_Thread->context), &(Curr_Thread->context));//swap the context from Prev_Thread to the thread pointed to Curr_Thread    
}

// Unblocks a thread by deleting it from the semaphore queue and adds it to the ReadyQ
void unblock_tcb(Semaphore_t *sem){
    AddQueue(ReadyQ, DelQueue(sem->SemQ));
}

// Allocates memory for a semaphore structure, creates a new semaphore queue and inititalizes the count value of the semaphore
Semaphore_t* CreateSem(int InputValue)
{   
    Semaphore_t *sem_st;
    sem_st = (Semaphore_t*)malloc(sizeof(Semaphore_t));
    
    sem_st->s_head = newQueue();
    sem_st->SemQ = &(sem_st->s_head);    

    sem_st->count = InputValue;
    return sem_st;
}

/*
 * The P routine decrements the semaphore, and if the value is less than
 * zero then blocks the process 
 */
void P(Semaphore_t *sem)
{   
    sem->count = sem->count - 1;
    if(sem->count < 0)
        block_tcb(sem);  
}

/*
 * The V routine increments the semaphore, and if the value is 0 or
 * negative, wakes up a process and yields
 */

void V(Semaphore_t * sem)
{   
    sem->count = sem->count + 1;
    if(sem->count <= 0)
        unblock_tcb(sem);
    yield();
}




