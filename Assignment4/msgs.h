/*
CSE 531: DMOS Project 4
Team Members:  Meenal Khandelwal  [ 1215375473 ] 
               Spurthi Sunil Madhure  [ 1215376084 ]

*/
#include "sem.h"


typedef struct arr{
    int a[10];
}arr; // message containing 10 integers array 

typedef struct port{ // Port contain N = 10 messgages
    arr msg[10];
    Semaphore_t  *empty, *full, *mutex;
    int send_idx;   // keep the record of index of the messages in the port sent
    int rcv_idx;
    }port;

port set[100];// Assume number of the port is an index

void init_port(){
    printf("Initializing the port\n");
    for(int i = 0; i < 100; i++){
        set[i].empty = CreateSem(10);
        set[i].full = CreateSem(0);
        set[i].mutex = CreateSem(1);
        set[i].send_idx = 0;
        set[i].rcv_idx = 0;
        
    }
    printf("End of init port\n");
    
}


int send(int port, int data[10]){

        
    P(set[port].empty);
    P(set[port].mutex);
    for(int i = 0; i < 10; i++){
        set[port].msg[(set[port].send_idx)].a[i] = data[i];
    }
    
    
    set[port].send_idx = (set[port].send_idx + 1)%10;
    V(set[port].mutex);
    V(set[port].full);  
    
 
    return 0;  
}

void rcv(int port, int result[10]){
    P(set[port].full);
    P(set[port].mutex);
    for(int i = 0; i < 10; i++){
        result[i] = set[port].msg[(set[port].rcv_idx)].a[i];
    }

    set[port].rcv_idx = (set[port].rcv_idx + 1)%10;
    V(set[port].mutex);
    V(set[port].empty);
}


 
