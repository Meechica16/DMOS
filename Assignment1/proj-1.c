/*
CSE 531: DMOS Project 1
Team Members:  Meenal Khandelwal  [ 1215375473 ] 
               Spurthi Sunil Madhure  [ 1215376084 ]

Compile instruction: gcc -pthread -Wall proj-1.c
*/

#define _GNU_SOURCE
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "sem.h"

semaphore_t add, mutex, child;
int count = 0;                                             // to keep track of number of threads that have finished addition
pthread_cond_t print = PTHREAD_COND_INITIALIZER;           // used to signal that print is completed
pthread_mutex_t thread_lock = PTHREAD_MUTEX_INITIALIZER;   // to ensure critical sections in different threads don't overlap

int arr[3];         

// thread callback function
void func(int *arg){
    while(1)
    {
        P(&mutex);
        (*arg)++;           // increments element of the array
        count++;            // increments the count
        //printf("Finished adding for child function %d\n",count);  // For Debug
         
        pthread_mutex_lock(&thread_lock);   // critical section 
       
        if(count == 3)     // if all 3 theads have finished adding
        {              
            //printf("Count in function: %d\n", count);  // For Debug
            //printf("Unlocking mutex add\n");  // For Debug
            V(&add);       // unlock 'add' causing the waiting main thread to acquire the lock 
            V(&mutex);     // unlock 'mutex' allowing the next child thread to acquire it
            //printf("if: child done adding and going to wait\n");  // For Debug
        }
        else
        {
            V(&mutex);    // unlock 'mutex' allowing the next child thread to acquire it
            //printf("else: child done adding and going to wait\n");  // For Debug
        }
        pthread_cond_wait(&print, &thread_lock);  // Wait for signal from main thread indicating print is complete before continuing with addition

        pthread_mutex_unlock(&thread_lock); 
        //printf("Child wakes up to repeat adding\n");  // For Debug
    }
}

int main(){

   init_sem(&add, 0);          // to provide syncronization that all children threads have completed addition 
   init_sem(&mutex, 1);         

   arr[0] = 0; arr[1] = 0; arr[2] = 0;  // initializes array values to 0

   // creating 3 children
   start_thread(func, &arr[0]); // passing address of each element of array as argument
   start_thread(func, &arr[1]);
   start_thread(func, &arr[2]);

   while(1)
   {
      //printf("Main: Waiting for add to complete\n");  // For Debug
      P(&add);         // wait till all add is done
      P(&mutex);       
      printf("Array values: %d, %d, %d\n", arr[0], arr[1], arr[2]);
      count = 0;      // reset count
      //printf("Count reset: %d\n",count);  // For Debug
      V(&mutex);
      //printf("Main: Send broadcast\n");   // For Debug
      
      pthread_mutex_lock(&thread_lock);    // critical section
      //sleep(1);  // to slow down the print output on terminal  
      pthread_cond_broadcast(&print);      //broadcast that print is done
      pthread_mutex_unlock(&thread_lock);
   }

   return 0;
}

