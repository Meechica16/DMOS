#include "q.h"

q_element *head;

q_element *NewItem(){
    q_element *item;
    item = (q_element*)malloc(sizeof(q_element));
    item -> next = NULL;
    item -> prev = NULL;
    item -> payload = 0;
    return *item;
}


void newQueue(){
    head = NULL;   
}

void AddQueue(*q_element head, *q_element item){
    if(head == NULL){
        head = item;
        item -> next = item -> prev;
        item -> prev = item -> next;
    }
}
