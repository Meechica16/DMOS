/*
CSE 531: DMOS Project 5
Team Members:  Meenal Khandelwal  [ 1215375473 ] 
               Spurthi Sunil Madhure  [ 1215376084 ]
	
compile instructions: gcc file_test.c
*/
#include "msgs.h"
#define TOTAL_PORT 100 
#include<unistd.h>
#include<stdlib.h>
#include<string.h>


int port_num = 0;

char fname[99][20];

Semaphore_t *cs_mutex;
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
    char *ch;

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
    //printf("Filename: %s\n",str);
    name_len ++;
    //printf("name length = %d\n",name_len);    
    name_integers = (name_len % 4 ==0)? (name_len/4):((name_len/4) +1); // Calculates the number of integers required to store the filename
    //printf("Num of integers for filename %d\n",name_integers);

    // put name into data
    for(int i=0; i<name_integers; i++){
        data[3+i] = *(unsigned int*)(str+(4*i));
        //printf("Data %d\n",data[3+i]);
    }

    //char *p = (char*)data;

    send(99, data);    
    rcv(rport, result);
    if(result[1] == -1){
        printf("%s: Size greater then 1MB \n",str);
    }
    else if(result[1] == -2){
        printf("%s: do not do that\n",str);
    }
    else if(result[1] == -3){
        printf("%s: FIlename is greater rejected by server\n",str);
    }
    else if(result[1] == 1){
        printf("%s: Client started sending data to server\n", str);
        data[0] = rport ; //0th element will have the reply port 
        data[1] = 1;
        int remainder = num_bytes%4;
        char *buffer = (char *)calloc(num_bytes+remainder,sizeof(char));
        fread(buffer, sizeof(char), num_bytes, fptr);
        //printf("Buffer data: \n%s",buffer);
        ch = buffer;
        while(!end_flag){
            //memset the data
            memset((data+2), '\0', 32); 
            for(int i=0; i<8; i++){
               data[2+i] = *(unsigned int*)(buffer);
               if(data[2+i] == 0){
                   printf("%s: End of File\n", str);
                   end_flag = 1;
                   break;
               }
               //printf("Data %d\n",data[2+i]);
               buffer = buffer + 4;
               }
            send(99, data);
            yield();
            }
            
        }


    free(ch);
    while(1)
        yield();
    
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

    while(1){
        rcv(99, data);
        int rport = 0; //rport of a client we are working on

        char *p = (char*)data;  
        int count;//Nymber of client used
        // printf("Name: \n");
        // for(int i=0; i<40; i++){
        //     printf("Addr %p, %c\n",p,*p++);
        // }

        switch(data[1]){
            case 0:
                if (data[2] <= 1048576 && p[27] == '\0')  // If size <1MB
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
                        //if(c[15] != '\0'){
                            //printf("Filename is greater then 15 char\n");
                            //result[1] = -3;
                            //send(rport, result); // FIle is ready
                            //break;
                        //}
                        while(*c != '\0'){
                            *(name_info++) = *c++;
                        }
                        //printf("CASE 0: struct_idx %d\t",struct_idx);
                        //printf("File name stored in struct = %s\t",rport_arr[struct_idx].client_fname);
                        //printf("count value:%d\n", count);

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
                    if(p[27] != '\0')
                        result[1] = -3;
                    else
                        result[1] = -1;
                    send(rport, result);
                }
                break;

            case 1:
                // Traverse the array of structs to get struct_idx
                printf("Server getting the data from the port \n");
                
                for(int i = 0; i < 3; i++)
                {
                    if(rport_arr[i].rport == data[0]){  // Meaning server is working on that particular client
                        struct_idx = i; 
                        break;
                    }
                }

                FILE *server_file = fopen(rport_arr[struct_idx].client_fname, "a");

                char *c = (char*)(data + 2);  // pointer to the file_data we have received from client
                
                //printf("Data we write to server file: \n%s\n",c);
                fprintf(server_file, "%s", c);  // copy the file data to the file created by server
                fclose(server_file);

                if(*(c + 28) == '\0'){  //Entire file transfer is complete
                    //put rport as -1 and decrement count
                    //printf("File %s  Struct Index:%d, count:%d \n", rport_arr[struct_idx].client_fname, struct_idx, count);
                     
                    rport_arr[struct_idx].rport = -1;
                    count--;
                    printf("%s: Transfer is complete\n",rport_arr[struct_idx].client_fname) ;
                    memset(rport_arr[struct_idx].client_fname, 0, 22); 
                    
                    // fclose(server_file);
                    //exit(0);
                }
                break;
        }

        yield();
    }
    
}

int main(int argc, char* argv[]){
    int len;
    
    //TO DO - If argc<3 then print error
    int num_client = atoi(argv[1]);
    
    printf("%s\n", argv[2]);
    for(int i = 2; i < argc; i++){
        len = strlen(argv[i]);
        //if(len > 16)
            //len = 16;
        printf("filename%s\n", argv[i]);
        //printf("%d\n", i);

        strncpy(fname[i-2], argv[i], len);
        //printf("%s\t", (fname[i-2]));
        if(len <= 15)
           fname[i][15] = '\0'; //16th element
    }
    
    head = newQueue();
    ReadyQ = &head;
    
    // This program has 1 Server 3 Clients
    cs_mutex = CreateSem(1);  // to keep the track of number of clients
    init_port();
    start_thread(server);
    for(int i = 0; i < num_client; i++)
        start_thread(client);
    run();
    return 0;
       
} 
