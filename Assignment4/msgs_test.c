/*
CSE 531: DMOS Project 4
Team Members:  Meenal Khandelwal  [ 1215375473 ] 
               Spurthi Sunil Madhure  [ 1215376084 ]
	
compile instructions: gcc msgs_test.c
*/
#include "msgs.h"
#define TOTAL_PORT 100 
#include<unistd.h>

Semaphore_t *cs_mutex;

int port_num = 0;


int cl_id;

TCB_t *head;

void client(){
    int id = cl_id++;
    printf("Enter into client %d\n",id);
    int data[10];  // One extra element for reply port
    int result[10];

    P(cs_mutex);
    int rport = port_num++;
    if(rport >= TOTAL_PORT - 1)
        exit(1);
    V(cs_mutex);
    data[0] = rport; //0th element will have the reply port
    for(int i = 1; i < 10; i++)  // Data giving to each client is it's id number
        data[i] = id; 
    while(1){
            printf("\nData Sent to Server from Client %d:",id);
            for(int i = 1; i < 10; i++){
		    printf("\nData[%d] = %d \t", i, data[i]);
            }
            printf("\n");
	    send(99, data);
	    rcv(rport, result);
            printf("\nResult Received from Server for Client %d:\n",id);
     
	    for(int i = 1; i < 10; i++){
		    printf("\nResult[%d] = %d \t", i, result[i]);
            
             } 
             printf("\n"); 
    sleep(1);
    yield();
    }

   
    //while(1){
      //   printf("client while\t");
        //yield();
    //}
    
}

void server(){
    
    int data[10]; 
    int result[10];
    int rport = 0;
 
    while(1){
        //printf("while loop\n");
        rcv(99, data);
        rport = data[0];
        for(int i = 1; i < 10; i++){ // In Server inreasing each element by 1
           // printf("\nSER Data: %d \t", data[i]);
            result[i] = data[i] + 1;
        }
        send(rport, result);
    }
    
}

int main(){

    head = newQueue();
    ReadyQ = &head;

    printf("head %p\n",head);
    
    // This program has 1 Server 3 Clients
    cs_mutex = CreateSem(1);  // to keep the track of number of clients
    init_port();
    start_thread(server);
    start_thread(client);
    start_thread(client);
    start_thread(client);
    run();
    return 0;
       
} 
