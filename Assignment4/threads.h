/*
CSE 531: DMOS Project 2
Team Members:  Meenal Khandelwal  [ 1215375473 ] 
               Spurthi Sunil Madhure  [ 1215376084 ]
Description: 
Implements the functions start_thread, run and yield.
*/

#include "q.h"

void start_thread(void (*function)(void));
void run();
void yield();

TCB_t **ReadyQ;      // points to the head pointer of Queue of TCBs
TCB_t *Curr_Thread;  // points to the current TCB
int counter = 0;     // counter for thread_id

// function which creates a stack, creates a TCB, initializes the TCB and adds it to the Queue
void start_thread(void (*function)(void)){
    void *stackP;
    TCB_t *tcb;
    stackP = (void*)malloc(8192);
    tcb = NewItem();
    init_TCB(tcb, function, stackP, 8192);
    tcb->thread_id = ++counter; 
    AddQueue(ReadyQ, tcb);
}
 
// function which runs the TCBs in the queue
void run(){
    Curr_Thread = DelQueue(ReadyQ);
    ucontext_t parent;    // get a place to store the main context, for faking
    getcontext(&parent);  // get the context of the parent
    swapcontext(&parent, &(Curr_Thread->context)); // swap context from parent to start the first thread
}

// function yields from curent TCB causing next TCB in the queue to run
void yield(){
    TCB_t *Prev_Thread;
    AddQueue(ReadyQ, Curr_Thread);  // Adds the current thread to the readyQ
    Prev_Thread = Curr_Thread;      // Stores address of current thread as previous thread
    Curr_Thread = DelQueue(ReadyQ); // Selects the next thread to run by deleting a TCB from the queue
    swapcontext(&(Prev_Thread->context), &(Curr_Thread->context));//swap the context from Prev_Thread to the thread pointed to Curr_Thread
}
