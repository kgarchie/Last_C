#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include "helper.h"
#include "myqueue.h"

void initQueue(Queue *q) {
    q->head = NULL;
    q->tail = NULL;
    q->size = 0;
}

int isEmpty(Queue *q) {
    if (q->size == 0) {
        return 1;
    } else {
        return 0;
    }
}

int enqueue(Queue *q, int data) {
    Node *newNode;
    newNode = (Node *)malloc(sizeof(Node));
    newNode->data = data;
    newNode->next = NULL;
    if (isEmpty(q)) {
        q->head = newNode;
        q->tail = newNode;
    } else {
        q->tail->next = newNode;
        q->tail = newNode;
    }
    q->size++;
    return data;
}

int dequeue(Queue *q) {
    Node *temp;
    int data;
    if (isEmpty(q)) {
        printf("Queue is empty\n");
        return;
    }
    temp = q->head;
    data = temp->data;
    q->head = q->head->next;
    if (q->head == NULL) {
        q->tail = NULL;
    }
    q->size--;
    free(temp);
    return data;
}

int peek(Queue *q) {
    if (isEmpty(q)) {
        printf("Queue is empty\n");
        return;
    }
    return q->head->data;
}

int size(Queue *q) {
    return q->size;
}