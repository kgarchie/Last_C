#ifndef MYQUEUE_H
#define MYQUEUE_H

// Creating a new type called Node.
typedef struct node {
    int data;
    struct node *next;
} Node;

// Creating a new type called Queue.
typedef struct queue {
    Node *head;
    Node *tail;
    int size;
} Queue;

// Defining the functions that will be used in the queue.
void initQueue(Queue *);
int isEmpty(Queue *);
int enqueue(Queue *, int);
int dequeue(Queue *);
int peek(Queue *);
int size(Queue *);

#endif
