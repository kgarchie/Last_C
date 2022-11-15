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

/**
 * It initializes the queue.
 *
 * @param q The queue to initialize.
 */
void initQueue(Queue *q)
{
    q->head = NULL;
    q->tail = NULL;
    q->size = 0;
}

/**
 * It checks if the queue is empty.
 *
 * @param q The queue to check.
 *
 * @return the size of the queue.
 */
int isEmpty(Queue *q)
{
    if (q->size == 0)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * If the queue is empty, then the new node becomes the head and tail of the queue, otherwise the new
 * node becomes the new tail of the queue
 *
 * @param q The queue to which we want to add the data.
 * @param data The data to be inserted into the queue.
 *
 * @return The data that was enqueued.
 */
int enqueue(Queue *q, int data)
{
    Node *newNode;
    newNode = (Node *)malloc(sizeof(Node));
    newNode->data = data;
    newNode->next = NULL;
    if (isEmpty(q))
    {
        q->head = newNode;
        q->tail = newNode;
    }
    else
    {
        q->tail->next = newNode;
        q->tail = newNode;
    }
    q->size++;
    return data;
}

/**
 * It removes the first element from the queue and returns it.
 *
 * @param q The queue to dequeue from.
 *
 * @return The data of the node that is being dequeued.
 */
int dequeue(Queue *q)
{
    Node *temp;
    int data;
    if (isEmpty(q))
    {
        printf("Queue is empty\n");
        return;
    }
    temp = q->head;
    data = temp->data;
    q->head = q->head->next;
    if (q->head == NULL)
    {
        q->tail = NULL;
    }
    q->size--;
    free(temp);
    return data;
}

/**
 * It returns the first element of the queue.
 *
 * @param q A pointer to the queue.
 *
 * @return The data of the head node.
 */
int peek(Queue *q)
{
    if (isEmpty(q))
    {
        printf("Queue is empty\n");
        return;
    }
    return q->head->data;
}

/**
 * It returns the size of the queue.
 *
 * @param q The queue to check the size of.
 *
 * @return The size of the queue.
 */
int size(Queue *q)
{
    return q->size;
}