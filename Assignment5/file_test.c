/*
CSE 531: DMOS Project 4
Team Members:  Meenal Khandelwal  [ 1215375473 ] 
               Spurthi Sunil Madhure  [ 1215376084 ]
	
compile instructions: gcc msgs_test.c
*/
#include "msgs.h"
#define TOTAL_PORT 100 
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#define COL 15

Semaphore_t *cs_mutex;

int port_num = 0;

char fname[99][15];


int cl_id;

TCB_t *head;

void client(){
    FILE *fptr;
    char *str;
    int id = cl_id++;
    printf("Enter into client %d\n",id);
    int data[10];  // One extra element for reply port
    int result[10];
    int name_len;
    int name_integers;

    P(cs_mutex);
    int rport = port_num++;
    if(rport >= TOTAL_PORT - 1)
        exit(1);
    V(cs_mutex);
    str = fname[id]; // Get the file name
   //Sending the size
    fptr = fopen(str, "r");
    if(fptr == NULL){
        printf("Cannot read file\n");
        return;
    }

    fseek(fptr,0L, SEEK_END);
    int num_bytes = ftell(fptr);
    printf("Number of bytes in file:%d\n", num_bytes);

    fseek(fptr,0L, SEEK_SET);





    // Sending the name
    data[0] = rport ; //0th element will have the reply port 
    data[1] = 0; //1st element tells name/data. if 0 then name.
    data[2] = num_bytes;
    
    name_len = strlen(str);
    printf("Filename: %s\n",str);
    name_len ++;
    printf("name length = %d\n",name_len);    
    name_integers = (name_len % 4 ==0)? (name_len/4):((name_len/4) +1); // Calculates the number of integers required to store the filename
    printf("Num of integers for filename %d\n",name_integers);

    // put name into data
    for(int i=0; i<name_integers; i++){
        data[3+i] = *(unsigned int*)(str+(4*i));
        printf("Data %d\n",data[3+i]);
    }

    char *p = (char*)data;

    printf("Name: \n");
    for(int i=0; i<40; i++){
        printf("Addr %p, %c\n",p,*p++);
    }
    send(99, data);    
    rcv(rport, result);
    if(result[1] == -1)
    {
        printf("Size greater then 1MB \n");
    }
    else if(result[1] == -2){
        printf("do not do that\n");
    }
    else{
        //Send the file
    }


    /*for(int i = 1; i < 10; i++)  // Data giving to each client is it's id number
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
    }*/
    yield();
   
    while(1){
      //   printf("client while\t");
        yield();
    }
    
}

void server(){
    
    int data[10]; 
    int result[10];
    int rport_arr[3] = {-1,-1,-1}; // Only serve three client at a time
    rcv(99, data);
    //Semaphore_t *count_mutex;  // One port update rport_arr

    char *p = (char*)data;
    int count;//Nymber of client used
    printf("Name: \n");
    for(int i=0; i<40; i++){
        printf("Addr %p, %c\n",p,*p++);
    }

    switch(data[1]){
        case 0:
            if (data[2] <= 1048576)
            {
                if (count < 3)
                {
                    for(int i = 0; i < 3; i++){
                        if(rport_arr[i] == -1){
                            rport_arr[i] = data[0];
                            count++;
                            break;
                    }
                    // Got the position now 
                    
                }
                else{
                    rport = data[0];
                    result[1] = -2;
                    send(rport, result);
                }

            }
            else{
                rport = data[0];
                result[1] = -1;
                send(rport, result);
            }
    }

    printf("Number of bytes:%d \n",data[2]);
    // Convert the data to string
 
    while(1){
      //   printf("client while\t");
        yield();
    }
    /*while(1){
        //printf("while loop\n");
        rcv(99, data);
        if((data[0] == rport[0]) || (data[0] == rport[0]))
        //rport[] = data[0];
        for(int i = 1; i < 10; i++){ // In Server inreasing each element by 1
           // printf("\nSER Data: %d \t", data[i]);
            result[i] = data[i] + 1;
        }
        send(rport, result);
    }*/
    
}

int main(int argc, char* argv[]){
    int len;
    int num_client = atoi(argv[1]);
    //fname = (char*)malloc((COL+1)*num_client*sizeof(char));
    printf("%s\n", argv[2]);
    for(int i = 2; i < argc; i++){
        len = strlen(argv[i]);
        if(len > 15)
            len = 15;
        printf("filename%s\n", argv[i]);
        //printf("%d\n", i);

        strncpy(fname[i-2], argv[i], len);
        //printf("%s\t", (fname[i-2]));
        fname[i][14] = '\0';
    }
    //printf("Hey\n");
    //str = fname + 1;
    //printf("%s\t",(fname));
    
    //for(int i = 0; fname[0][i] != '\0'; i++)
    //    printf("%c, %d\t",fname[0][i], i);
    //exit(1);
    
    head = newQueue();
    ReadyQ = &head;

    printf("head %p\n",head);
    
    // This program has 1 Server 3 Clients
    cs_mutex = CreateSem(1);  // to keep the track of number of clients
    init_port();
    start_thread(server);
    start_thread(client);
    //start_thread(client);
    //start_thread(client);
    run();
    return 0;
       
} 
