
#include "threads.h"

TCB_t *head;
int g = 0;//global counter

void f1(){
    int count1 = 0;// local counter of f1
    while(1){
         printf("value of count 1: %d and global counter: %d\n", count1++, g++);
         printf("Yielding in f1\n");
         yield();
    }
}

void f2(){
    int count2 = 0;// local counter of f2
    while(1){
         printf("value of count 2: %d and global counter: %d\n", count2++, g++);
         printf("Yielding in f2\n");
         yield();
    }
}

void f3(){
    int count3 = 0;// local counter of f3
    while(1){
         printf("value of count 3: %d and global counter: %d\n", count3++, g++);
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
