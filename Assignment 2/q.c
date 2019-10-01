#include "q.h"

q_element *NewItem(){
    q_element *item;
    item = (q_element*)malloc(sizeof(q_element));
    item -> next = NULL;
    item -> prev = NULL;
    item -> payload = 0;
    return item;
}


q_element *newQueue(){
    head = NULL;
    return head;   
}

void AddQueue(q_element *head, q_element *item, int value){
    q_element *last_node;
    if(head == NULL){   //when we want to add to an empty queue
        head = item;
        head->next = item;
        head->prev = item;
        item->payload = value;
    }
    else{// when we have to add to the tail
        last_node = head->prev;  //storing address of tail in last_node
        head->prev = item;       //pointing prev of head node to the item node 
        last_node->next = item;       //pointing prev of tail node to the item node
        item->prev = last_node;  //pointing prev of item node to the tail node 
        item->next = head;       //pointing next of item node to the head node
        item->payload = value;
    }
}

q_element *DelQueue(q_element *head){
    q_element *last_node, *temp;
    temp = NULL;
    if(head == NULL)
        return temp;  // To Do Flag Error
    else if(head->next == head){
        temp = head;
        head->prev = NULL;
        head->next = NULL;
        head = NULL; 
    }
    else{
        temp = head;
        last_node = head->prev;
        head = head->next;
        last_node->next = head;
        head->prev = last_node;
    }
    return temp; // returning pointer to deleted item
}

void display(q_element *head){
    q_element *temp;
    temp = head;
    do{
        printf("Payload value:%d\n",temp->payload);
        temp = temp->next;
    }while(temp != head);
}


