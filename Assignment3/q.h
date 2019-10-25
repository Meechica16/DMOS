/*
CSE 531: DMOS Project 3
Team Members:  Meenal Khandelwal  [ 1215375473 ] 
               Spurthi Sunil Madhure  [ 1215376084 ]
Description: 
Implementing the following functions for a Queue:
    item = NewItem()      -> returns a pointer to a new q-element, uses memory allocation
    head = newQueue()     -> creates an empty queue, that is the header pointer is set to null.
    AddQueue(head, item)  -> adds a queue item, pointed to by “item”, to the queue pointed 
                            to by head.
    item = DelQueue(head) -> deletes an item from head and returns a pointer to the 
	                         deleted item. If the queue is already empty, flag error
*/

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include "TCB.h"

TCB_t *NewItem();
TCB_t *newQueue();
void AddQueue(TCB_t**, TCB_t*);
TCB_t *DelQueue(TCB_t**);
void display(TCB_t*);

// function to create a new TCB (q-element)
TCB_t *NewItem(){
    TCB_t *item;       // pointer to store address of created TCB
    item = (TCB_t*)malloc(sizeof(TCB_t));
    if(item == NULL)
        exit(0);
    item->next = NULL;
    item->prev = NULL;
    return item;      
}

// function to create a new queue, returns address of head pointer
TCB_t *newQueue(){
    TCB_t *head;
    head = (TCB_t*)malloc(sizeof(long));
    if(head == NULL)
        exit(0);
    head = NULL; 
    return head;   
}

// function to add a TCB to the queue
// **head_ptr is a pointer to the head 
void AddQueue(TCB_t **head_ptr, TCB_t *item){  
    TCB_t *last_node;
    if(*head_ptr == NULL)    // If queue is empty i.e if head is NULL
	{   
        *head_ptr = item;
        item->next = *head_ptr;
        item->prev = *head_ptr;
    }
    else   // If atleast one element present in queue, add to the tail
	{              
        last_node = (*head_ptr)->prev; // storing address of tail in last_node
        (*head_ptr)->prev = item;      // pointing prev of head node to the item node 
        last_node->next = item;        // pointing prev of tail node to the item node
        item->prev = last_node;        // pointing prev of item node to the tail node 
        item->next = (*head_ptr);      // pointing next of item node to the head node
    }
}

// function to delete a TCB from the queue
// **head_ptr is a pointer which points to the head 
TCB_t *DelQueue(TCB_t **head_ptr){  
    TCB_t *last_node, *temp;
    temp = NULL;
    if(*head_ptr == NULL){
        printf("Error: Queue is empty, cannot delete any element\n");
        exit(2);  // Flags an error
    }
    else if((*head_ptr)->next == (*head_ptr)){
        //printf("Del: last element remaining in the queue\n");  // For Debug
        temp = (*head_ptr);
        (*head_ptr)->prev = NULL;
        (*head_ptr)->next = NULL;
        (*head_ptr) = NULL; 
    }
    else{
        //printf("Del: more then one element remaining in the queue\n");  // For Debug
        temp = (*head_ptr);    
        last_node = (*head_ptr)->prev;
        (*head_ptr) = (*head_ptr)->next;
        last_node->next = (*head_ptr);
        (*head_ptr)->prev = last_node;
    }
    return temp; // returning pointer to deleted TCB
}

// function to display the thread IDs of all the threads in the queue
void display(TCB_t *head){
    TCB_t *temp;  // to iterate through the queue
    temp = head;  
    do{
        printf("Thread ID:%d\n",temp->thread_id);
        temp = temp->next;
    }while(temp != head);
}



