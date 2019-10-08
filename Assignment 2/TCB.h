/*
CSE 531: DMOS Project 2
Team Members:  Meenal Khandelwal  [ 1215375473 ] 
               Spurthi Sunil Madhure  [ 1215376084 ]
Description:
Type defines a structure TCB_t containing next pointer, previous pointer, thread_id and context 
init_TCB initializes the TCB with the stack pointer and the context

*/

#include <ucontext.h>
#include <string.h>

typedef struct TCB_t{
    struct TCB_t *next;
    struct TCB_t *prev;
    int thread_id;
    ucontext_t context;
}TCB_t;

// Initialization of TCB
void init_TCB(TCB_t *tcb, void *function, void *stackP, int stack_size){
    memset(tcb, '\0', sizeof(TCB_t));   // setting all of the elements to 0
    getcontext(&tcb->context);          // get context of the tcb
    tcb->context.uc_stack.ss_sp = stackP;  
    tcb->context.uc_stack.ss_size = (size_t) stack_size; 
    makecontext(&tcb->context, function, 0);    // context is now cooked
}
