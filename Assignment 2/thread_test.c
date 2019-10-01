#include "q.h"


q_element *head;

int main(){
    printf("Starting the main\n");
    q_element *item;
    printf("Creating new item 1\n");
    item = NewItem();
    printf("Creating new queue\n");
    head = newQueue();
    printf("Adding new item 1 to queue\n");
    AddQueue(head, item, 1);
    printf("Creating new item 2\n");
    item = NewItem();
    printf("Adding new item 2 to queue\n");
    AddQueue(head, item, 2);
    printf("Start printing\n");
    display(head);
    return 0;
}
