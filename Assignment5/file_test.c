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
    int end_flag = 0;
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
    if(result[1] == -1){
        printf("Size greater then 1MB \n");
    }
    else if(result[1] == -2){
        printf("do not do that\n");
    }
    else if(result[1] == 1){
        printf("Server has received name\n");
        data[0] = rport ; //0th element will have the reply port 
        data[1] = 1;
        int remainder = num_bytes%4;
        char *buffer = (char *)calloc(num_bytes+remainder,sizeof(char));
        fread(buffer, sizeof(char), num_bytes, fptr);
        printf("Buffer data: \n%s",buffer);
        
        while(!end_flag){
            //memset the data
            memset((data+2), '\0', 32); 
            for(int i=0; i<8; i++){
               data[2+i] = *(unsigned int*)(buffer);
               if(data[2+i] == 0){
                   printf("Got the mistake");
                   end_flag = 1;
                   break;
               }
               printf("Data %d\n",data[2+i]);
               buffer = buffer + 4;
               }
               //buffer = buffer + 4;
            send(99, data);
            yield();
            }
            
        }

        // for(int i=0; i<8; i++){
        //     do{
        //        data[2+i] = *(unsigned int*)(buffer);
        //        printf("Data %d\n",data[2+i]);
        //        buffer = buffer + 4;
        //        send(99, data);
        //        yield();
        //     }while(*buffer != '\0' );
        // }

        //rcv()
        //Send the file
    


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
    while(1)
        yield();
   
    // while(1){
    //   //   printf("client while\t");
    //     yield();
    // }
    
}

void server(){
    
    int data[10]; 
    int result[10];
    struct rport_info{
        int rport;  
        char client_fname[22];  // 22 so that we can add .server at the end of filename we get from client 
    };
    struct rport_info rport_arr[3];

    char append[] = ".server";

    int struct_idx = -1;
    
    for(int i=0; i < 3; i++){
        rport_arr[i].rport = -1;    
    } 

    //int rport_arr[3] = {-1,-1,-1}; // Only serve three client at a time
    while(1){
        rcv(99, data);
        int rport = 0; //rport of a client we are working on
        //Semaphore_t *count_mutex;  // One port update rport_arr

        char *p = (char*)data;  
        int count;//Nymber of client used
        // printf("Name: \n");
        // for(int i=0; i<40; i++){
        //     printf("Addr %p, %c\n",p,*p++);
        // }

        switch(data[1]){
            case 0:
                if (data[2] <= 1048576)  // If size <1MB
                {
                    if (count < 3)  // If number of concurrent clients < 3
                    {
                        for(int i = 0; i < 3; i++)
                        {
                            if(rport_arr[i].rport == -1){  // Meaning that particular position is empty
                                rport_arr[i].rport = data[0];  // get the rport from data and store it
                                struct_idx = i;
                                count++;
                                break;
                            }
                        }
                        // Got the position now, now we have to store the filename

                        char *c = (char*)(data+3);  // start reading from 1st char stored in client at 3rd integer
                        char *name_info = rport_arr[struct_idx].client_fname;
                        while(*c != '\0'){
                            *(name_info++) = *c++;
                        }
                        printf("File name stored in struct = %s\n",rport_arr[struct_idx].client_fname);
                        printf("Filename length %lu\n",strlen(rport_arr[struct_idx].client_fname));

                        // Done storing filename in rport_arr 
                        // Now create a file named filename.server
                        strncat(rport_arr[struct_idx].client_fname, append, 7);
                        FILE *server_file = fopen(rport_arr[struct_idx].client_fname, "w");
                        fclose(server_file);  // close the file after creating

                        // For Debug - To check whether we can actually write to the file we just created
                        // printf("\nWriting to file\n");
                        // char str_add[] = "hello server file";
                        // printf("Size of str_add = %d\n",sizeof(str_add));
                        // fprintf(server_file, "%s", str_add);
                        // fprintf(server_file, "%s", str_add);
                        // fclose(server_file);
                        rport = rport_arr[struct_idx].rport;
                        result[1] = 1;
                        send(rport, result); // FIle is ready
                    }
                    else{  // number of clients exceeds 3
                        rport = data[0];
                        result[1] = -2;
                        send(rport, result);
                    }

                }
                else{  // size > 1MB
                    rport = data[0];
                    result[1] = -1;
                    send(rport, result);
                }
                break;

            case 1:
                // Traverse the array of structs to get struct_idx
                printf("In case 1\n");
                
                for(int i = 0; i < 3; i++)
                {
                    if(rport_arr[i].rport == data[0]){  // Meaning server is working on that particular client
                        struct_idx = i; 
                        break;
                    }
                }

                FILE *server_file = fopen(rport_arr[struct_idx].client_fname, "a");

                char *c = (char*)(data + 2);  // pointer to the file_data we have received from client
                
                printf("Data we write to server file: \n%s\n",c);
                fprintf(server_file, "%s", c);  // copy the file data to the file created by server

                fclose(server_file);
                if(*(c + 28) == '\0'){  //Entire file transfer is complete
                    //put rport as -1 and decrement count
                    printf("Struct Index:%d, count:%d \n", struct_idx, count);
                    
                    rport_arr[struct_idx].rport = -1;
                    count--;
                    
                    memset(rport_arr[struct_idx].client_fname, 0, 22); 
                    printf("Closing the file\n");
                    // fclose(server_file);
                    //exit(0);
                }
                break;
        }

        yield();
    }

    printf("Number of bytes:%d \n",data[2]);
    // Convert the data to string
 
    //while(1){
      //   printf("client while\t");
      //  yield();
    //}
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
    
    //TO DO - If argc<3 then print error
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
