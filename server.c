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

#define MAX_SIZE 1024
#define PORT_NUMBER 5678
#define MAX_USERS 100
#define NUM_THREADS 10

void *clientHandler(void *arg);
// int userCount = 0;
// int userList[MAX_USERS];

/**
 * It creates a socket, binds it to a port, and then listens for incoming connections. When a
 * connection is made, it creates a new thread to handle the connection
 *
 * @param argc the number of arguments passed to the program
 * @param argv The argument vector.
 */
int main(int argc, char *argv[])
{
    int sockfd, newsockfd, portno, n;
    struct sockaddr_in serv_addr, cli_addr;
    socklen_t clilen;
    char buffer[MAX_SIZE];
    pthread_t threads[NUM_THREADS];
    int rc;
    int i;

    Queue q;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("ERROR opening socket");
        exit(1);
    }

    /* Setting the address to 0. */
    bzero((char *)&serv_addr, sizeof(serv_addr));
    /* Setting the port number to 5678, and setting the address family to AF_INET, and setting the address
    to INADDR_ANY, and setting the port number to 5678. */
    portno = PORT_NUMBER;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("ERROR on binding");
        exit(1);
    }

    listen(sockfd, NUM_THREADS);
    clilen = sizeof(cli_addr);

    initQueue(&q);
    /* Creating a thread for each client that connects to the server. */

    for (i = 0; i < NUM_THREADS; i++)
    {
        newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
        if (newsockfd < 0)
        {
            perror("ERROR on accept");
            exit(1);
        }

        rc = pthread_create(&threads[i], NULL, clientHandler, (void *)newsockfd);

        if (rc)
        {
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
    }

    pthread_exit(NULL);
}

/**
 * It reads a command from the client, and then executes the command
 *
 * @param arg This is the argument passed to the thread function.
 */
void *clientHandler(void *arg)
{
    int n;
    int newsockfd = (int)arg;
    int i;
    int userid;
    int numusers;
    int found;
    char buffer[MAX_SIZE];
    char command[MAX_SIZE];
    char response[MAX_SIZE];
    char username[MAX_SIZE];
    char password[MAX_SIZE];
    char userlist[MAX_SIZE];
    char *token;
    char *pch;

    // int userCount = 0;
    // int userList[MAX_USERS];

    Queue q;

    initQueue(&q);

    /* Reading the command from the client and then executing the command. */
    while (1)
    {
        n = read(newsockfd, buffer, MAX_SIZE);
        if (n < 0)
        {
            perror("ERROR reading from socket");
            exit(1);
        }
        /* Splitting the string into tokens. */
        token = strtok(buffer, " ");
        /* The below code is adding a user to the 'database'. */
        if (strcmp(token, "ADD") == 0)
        {
            /* Splitting the string into tokens. */
            token = strtok(NULL, " ");
            /* Checking if the token is not null, and if it is not null, it is copying the token into
            the username variable. */
            if (token != NULL)
            {
                strcpy(username, token);
            }
            /* Splitting the string into tokens. */
            token = strtok(NULL, " ");
            if (token != NULL)
            {
                strcpy(password, token);
            }
            userid = addUser(username, password);
            if (userid > 0)
            {
                strcpy(response, "OK");
            }
            else
            {
                strcpy(response, "ERROR");
            }
            n = write(newsockfd, response, MAX_SIZE);
            if (n < 0)
            {
                perror("ERROR writing to socket");
                exit(1);
            }
        }
            /**
             * It searches for a user in the database and returns a response to the client
             *
             * @param  `username` - the username of the user to be searched
             */
        else if (strcmp(token, "SEARCH") == 0)
        {
            token = strtok(NULL, " ");
            if (token != NULL)
            {
                strcpy(username, token);
            }
            found = searchUser(username);
            if (found > 0)
            {
                strcpy(response, "FOUND");
            }
            else
            {
                strcpy(response, "NOTFOUND");
            }
            n = write(newsockfd, response, MAX_SIZE);
            if (n < 0)
            {
                perror("ERROR writing to socket");
                exit(1);
            }
        }
            /**
             * It's a function that deletes a user from the database
             *
             * @param  `username` - the username of the user to be deleted
             */
        else if (strcmp(token, "DELETE") == 0)
        {
            token = strtok(NULL, " ");
            if (token != NULL)
            {
                strcpy(username, token);
            }
            found = deleteUser(username);
            if (found > 0)
            {
                strcpy(response, "OK");
            }
            else
            {
                strcpy(response, "ERROR");
            }
            n = write(newsockfd, response, MAX_SIZE);
            if (n < 0)
            {
                perror("ERROR writing to socket");
                exit(1);
            }
        }
            /**
             * It takes a string and a socket, and sends the string to the socket
             *
             * @param  `token` is the command that the client sent to the server.
             */
        else if (strcmp(token, "LIST") == 0)
        {
            strcpy(userlist, "");
            numusers = listUsers(userlist);
            n = write(newsockfd, userlist, MAX_SIZE);
            if (n < 0)
            {
                perror("ERROR writing to socket");
                exit(1);
            }
        }
            /**
             * It reads a line from the socket, and if the line is "EXIT", it closes the socket and exits the
             * thread
             *
             * @param  `newsockfd` is the socket descriptor for the client.
             */
        else if (strcmp(token, "EXIT") == 0)
        {
            close(newsockfd);
            pthread_exit(NULL);
        }
    }
}

// usage: ./server