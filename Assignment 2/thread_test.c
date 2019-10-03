
#include "threads.h"

TCB_t *head;

void f1(){
    while(1){
         //printf("Printing F1\n");
         //display(head);
         printf("Yielding in f1\n");
         yield();
    }
}

void f2(){
    while(1){
         //printf("Printing F2\n");
         //display(head);
         printf("Yielding in f2\n");
         yield();
    }
}

void f3(){
    while(1){
         //printf("Printing F3\n");
         //display(head);
         printf("Yielding in f3\n");
         yield();
    }
}



int main(){
    
    ReadyQ = &head;
    start_thread(f1);
    start_thread(f2);
    start_thread(f3);
    run();
    return 0;
}
