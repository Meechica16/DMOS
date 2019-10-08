/*
CSE 531: DMOS Project 2
Team Members:  Meenal Khandelwal  [ 1215375473 ] 
               Spurthi Sunil Madhure  [ 1215376084 ]
Description: 
Contains 3 functions which are called by three threads
Main thread creates a newQueue, starts 3 threads and calls the run() function which 
    runs the threads from the Queue
	
compile instructions: gcc thread_test.c
*/

#include "threads.h"

TCB_t *head;  // head pointer
int g = 0;    // global counter
	
void f1(){
    int count1 = 0;    // local counter of f1
    while(1){
        printf("Function1: local count1=%d and global counter=%d\n", count1++, g++);
        printf("Yielding in f1\n");
        yield();
    }
}

void f2(){
    int count2 = 0;     // local counter of f2
    while(1){
        printf("Function2: local count2=%d and global counter=%d\n", count2++, g++);
        printf("Yielding in f2\n");
        yield();
    }
}

void f3(){
    int count3 = 0;     // local counter of f3
    while(1){
        printf("Function3: local count3=%d and global counter=%d\n", count3++, g++);
        printf("Yielding in f3\n");
        yield();
    }
}

int main(){
    ReadyQ = &head;     // ReadyQ stores the address of the head pointer
    head = newQueue();  // Creating a queue which returns address of head pointer
    start_thread(f1);   // Thread 1 with callback function f1
    start_thread(f2);   // Thread 2 with callback function f2
    start_thread(f3);   // Thread 3 with callback functino f3
    run();              // Runs the TCBs from the queue one after the other
    return 0;
}
