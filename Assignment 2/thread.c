#include "threads.h"

TCB_t **ReadyQ;
TCB_t *Curr_Thread;
int counter = 0;

void start_thread(void (*function)(void)){
    void *stackP;
    TCB_t *tcb;
    stackP = (void*)malloc(8192);
    tcb = NewItem();
    init_TCB(tcb, function, stackP, 8192);
    tcb->thread_id = ++counter; 
    AddQueue(ReadyQ, tcb);
}
 
void run(){
    Curr_Thread = DelQueue(ReadyQ);
    ucontext_t parent; // get a place to store the main context, for faking
    getcontext(&parent); // magic sauce
    swapcontext(&parent, &(Curr_Thread->context)); // start the first thread
}

void yield(){
    TCB_t *Prev_Thread;
    AddQueue(ReadyQ, Curr_Thread);
    Prev_Thread = Curr_Thread;
    Curr_Thread = DelQueue(ReadyQ);
    swapcontext(&(Prev_Thread->context), &(Curr_Thread->context));//swap the context, from Prev_Thread to the thread pointed to Curr_Thread
}
