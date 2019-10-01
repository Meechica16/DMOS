//CSE 531: DMOS Project 2


#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
// Implementing Queue with the following functions
// item = NewItem() returns a pointer to a new q-element, uses memory allocation
// head = newQueue() // creates a empty queue, that is the header pointer is set to null.
// AddQueue(head, item) // adds a queue item, pointed to by “item”, to the queue pointed to by head.
// item = DelQueue(head) // deletes an item from head and returns a pointer to the deleted item. If the queue is already empty, flag error

// q-element 
typedef struct q_element{
    struct q_element *next;
    struct q_element *prev;
    int payload;    
}q_element;

q_element *NewItem();
q_element *newQueue();
void AddQueue(q_element *, q_element *, int);
q_element *DelQueue(q_element *);
void display(q_element *);

extern q_element *head;


