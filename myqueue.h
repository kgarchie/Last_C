#ifndef MYQUEUE_H
#define MYQUEUE_H

typedef struct node {
    int data;
    struct node *next;
} Node;

typedef struct queue {
    Node *head;
    Node *tail;
    int size;
} Queue;

void initQueue(Queue *);
int isEmpty(Queue *);
int enqueue(Queue *, int);
int dequeue(Queue *);
int peek(Queue *);
int size(Queue *);

#endif
