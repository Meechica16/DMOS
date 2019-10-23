// (c) Partha Dasgupta 2009
// permission to use and distribute granted.

#define _GNU_SOURCE
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "sem.h"

TCB_t *head;  // head pointer
Semaphore_t *r_sem, *w_sem, *mutex;
int wrc = 0, wwc = 0, rc = 0, wc = 0, global_ID=0;// waiting reader, waiting writer

void reader_entry(int ID)
{
	printf("[reader: #%d]\ttrying to read\n", ID);
	P(mutex);
	if (wwc > 0 || wc > 0) {  // If already have a write then let them sleep
	    printf("[reader: #%d]\tblocking for writer\n", ID);
	    wrc++;		// increment waiting reader count.
	    V(mutex);		// let other processes use the mutex.
	    P(r_sem);		// sleep on r_sem
	    wrc--;		// program now has mutex and r_sem, go
			        // into CS.
	}
	rc++;
	if (wrc > 0)   // If no waiting read are present then let go of mutex otherwise not
	    V(r_sem);
	else
	    V(mutex);		// let other processes use the mutex.
}

void reader_exit(int ID)
{
        printf("[reader: #%d]\treader exit\n", ID);
	P(mutex);
	rc--;			// i'm no longer a reader
	if (rc == 0 && wwc > 0) {
	    // if was the last reader, and there are waiting writers, open 
	    // the w_sem door for them.
	    V(w_sem);
	} else
	    V(mutex);

}

void writer_entry(int ID)
{
	printf("\t\t\t\t[writer: #%d]\ttrying to write\n", ID);
	P(mutex);
	if (rc > 0 || wc > 0) {
	    printf("\t\t\t\t[writer: #%d] blocking for others\n",ID);
	    wwc++;		// increment waiting writers
	    V(mutex);		// let go of the mutex, since i'll be
	    // blocked
	    P(w_sem);		// wait in my line, when i wake up i DON'T 
		 	    	// need a P(mutex since i've been given it 
	    			// by the waking process.
	    wwc--;		// i'm no longer waiting
	}
	wc++;			// increment writers
	V(mutex);		// let go of the mutex
}

void writer_exit(int ID)
{
        printf("\t\t\t\t[writer: #%d]\twriter exit\n", ID);
	P(mutex);
	wc--;
	if (wrc > 0) {		// first, i let all the readers go. Before jumping to next write if some write is waiting then serve those first
	    V(r_sem);
	} else if (wwc > 0) {	// then i let another writer go
	    V(w_sem);
	} else {		// lastly, i give up the mutex
	    V(mutex);
	}
}



void reader(void)
{ 
  int ID;
  P(mutex); 
  ID = global_ID++; 
  V(mutex);
  while(1){
	reader_entry(ID);
	printf("[reader #%d]\t****READING****\n", ID);
	sleep(1);
	reader_exit(ID);
  };
}

void writer(void)
{
  int ID;
  P(mutex); 
  ID = global_ID++; 
  V(mutex);
  while(1){
 	writer_entry(ID);
	printf("\t\t\t\t[writer: #%d]\t&&&WRITING!&&&\n", ID);
	sleep(1);
	writer_exit(ID);
  };
}



//-------------------------------------------------------

int main()
{
    ReadyQ = &head;     // ReadyQ stores the address of the head pointer
    head = newQueue();  // Creating a queue which returns address of head pointer
    mutex = CreateSem(1);
    r_sem = CreateSem(0);
    w_sem = CreateSem(0);
    start_thread(reader);
    start_thread(reader);
    start_thread(reader);
    start_thread(reader);
    start_thread(writer);
    start_thread(writer);
    run();
    return 0;
}   



