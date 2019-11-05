#include "msgs.h"
#define TOTAL_PORT 100

Semaphore_t *cs_mutex;

int port_num = 0;
int pivot = 0;

int cl_id;

TCB_t *head;

void client(){
    int id = cl_id++;
    printf("Enter into client %d\n",id);
    int data[10];  // One extra element for reply port
    int result[10];

    P(cs_mutex);
    int rport = port_num++;
    if(rport >= TOTAL_PORT - pivot)
        exit(1);
    V(cs_mutex);
    data[0] = rport; //0th element will have the reply port
    for(int i = 1; i < 10; i++)
        data[i] = id; 
    send(99, data);
    rcv(rport, result);
    for(int i = 1; i < 10; i++){
            printf("\nCLT %d Result: %d \t", id, result[i]);
    }
   
    while(1){
      //   printf("client while\t");
        yield();
    }
    
}

void server(){
    pivot++;
    int sp = TOTAL_PORT - 1; 
    int data[10]; 
    int result[10];
    int rport = 0;
 
    while(1){
        //printf("while loop\n");
        rcv(99, data);
        rport = data[0];
        for(int i = 1; i < 10; i++){
            printf("\nSER Data: %d \t", data[i]);
            result[i] = data[i] + 1;
        }
        send(rport, result);
    }
    
}



int main(){

    ReadyQ = &head;
    head = newQueue();

    cs_mutex = CreateSem(1);
    init_port();
    start_thread(client);
    start_thread(client);
    start_thread(client);
    start_thread(server);
    run();
    return 0;
       
} 
