//CSE 531: DMOS Project 2


#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include "TCB.h"
// Implementing Queue with the following functions
// item = NewItem() returns a pointer to a new q-element, uses memory allocation
// head = newQueue() // creates a empty queue, that is the header pointer is set to null.
// AddQueue(head, item) // adds a queue item, pointed to by “item”, to the queue pointed to by head.
// item = DelQueue(head) // deletes an item from head and returns a pointer to the deleted item. If the queue is already empty, flag error

// TCB element
/*typedef struct TCB_t{
    struct TCB_t *next;
    struct TCB_t *previous;
    int thread_id;
    ucontext_t context;
}TCB_t;*/


// q-element 
/*typedef struct q_element{
    struct q_element *next;
    struct q_element *prev;
    //int payload;    
}q_element;*/



TCB_t *NewItem();
TCB_t *newQueue();
void AddQueue(TCB_t**, TCB_t*);
TCB_t *DelQueue(TCB_t**);
void display(TCB_t*);

//extern q_element *head;

TCB_t *NewItem(){
    TCB_t *item;
    item = (TCB_t*)malloc(sizeof(TCB_t));
    if(item == NULL)
        exit(0);
    item->next = NULL;
    item->prev = NULL;
    //item->payload = 0;
    return item;
}


TCB_t *newQueue(){
    TCB_t *head;
    head = (TCB_t*)malloc(sizeof(long));
    if(head == NULL)
        exit(0);
    head = NULL;
    return head;   
}

void AddQueue(TCB_t **head, TCB_t *item){
    TCB_t *last_node;
    if(*head == NULL){   //when we want to add to an empty queue
        *head = item;
        //printf("Payload value:%d\n",item->payload);
        item->next = *head;
        item->prev = *head;
        //item->payload = value;
        //printf("Payload value:%d\n",(*head)->payload);
    }
    //printf("Payload value:%d\n",temp->payload);
    else{// when we have to add to the tail
        //printf("AM I even creating list");
        last_node = (*head)->prev;  //storing address of tail in last_node
        (*head)->prev = item;       //pointing prev of head node to the item node 
        last_node->next = item;       //pointing prev of tail node to the item node
        item->prev = last_node;  //pointing prev of item node to the tail node 
        item->next = (*head);       //pointing next of item node to the head node
        //item->payload = value;
    }
}

TCB_t *DelQueue(TCB_t **head){
    TCB_t *last_node, *temp;
    temp = NULL;
    if(*head == NULL){
        printf("Error: Nothing to delete\n");
        exit(2);  // To Do Flag Error
    }
    else if((*head)->next == (*head)){
        //printf("Del: Only one element is remaining\n");
        temp = (*head);
        (*head)->prev = NULL;
        (*head)->next = NULL;
        (*head) = NULL; 
    }
    else{
        //printf("Del: more then one element is remaining\n");
        temp = (*head);
        last_node = (*head)->prev;
        (*head) = (*head)->next;
        last_node->next = (*head);
        (*head)->prev = last_node;
    }
    return temp; // returning pointer to deleted item
}

void display(TCB_t *head){
    //printf("OK! I am inside display\n");
    //printf("Payload value:%d\n",head->payload);
    TCB_t *temp;
    temp = head;
    do{
        //printf("OK! I am inside display");
        printf("Payload value:%d\n",temp->thread_id);
        temp = temp->next;
    }while(temp != head);



}



