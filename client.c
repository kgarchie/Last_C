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
#define SERVER_ADDR "127.0.0.1"

int main(int argc, char *argv[])
{
    /* Declaring variables */
    int sockfd;
    int n;
    int i;
    int choice;
    int userid;
    int numusers;
    int found;
    struct sockaddr_in serv_addr;

    /* Declaring the variables that will be used in the program. */
    char buffer[MAX_SIZE];
    char command[MAX_SIZE];
    char response[MAX_SIZE];
    char username[MAX_SIZE];
    char password[MAX_SIZE];
    char userlist[MAX_SIZE];

    /* Used to tokenize the string. */
    char *token;
    char *pch;

    /* Declaring a queue. */
    Queue q;

    /* Creating a socket. */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0)
    {
        perror("ERROR opening socket");
        exit(1);
    }

    /* Setting the memory of the serv_addr to 0. */
    bzero((char *)&serv_addr, sizeof(serv_addr));

    /* Setting the address family to IPv4. */
    serv_addr.sin_family = AF_INET;

    /* Converting the port number to network byte order. */
    serv_addr.sin_port = htons(PORT_NUMBER);
    serv_addr.sin_addr.s_addr = inet_addr(SERVER_ADDR);

    /* Connecting to the server. */
    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("ERROR connecting");
        exit(1);
    }

    printf("Connected to server\n");

    initQueue(&q);

    /* This is the main menu of the client program. It allows the user to choose from a list of
    options. */
    while (1)
    {
        printf("\nEnter a command:\n");
        printf("1. Add User\n");
        printf("2. Search Users\n");
        printf("3. Delete Users\n");
        printf("4. List Users\n");
        printf("5. Exit\n =========================================== \n");
        scanf("%d", &choice);

        switch (choice)
        {
            /* This is the case for the add command. It sends the command to the server and then reads the
                   response. The response is a string of users separated by commas. The string is then tokenized
                   and printed. */
            case 1:
                printf("Enter a username: ");
                scanf("%s", username);
                printf("Enter a password: ");
                scanf("%s", password);

                /* Creating the command to send to the server. */
                sprintf(command, "ADD %s %s", username, password);

                /* Sending the command to the server. */
                write(sockfd, command, MAX_SIZE);

                /* Reading the response from the server. */
                n = read(sockfd, response, MAX_SIZE);
                if (n < 0)
                {
                    perror("ERROR reading from socket");
                    exit(1);
                }
                if (strcmp(response, "OK") == 0)
                {
                    printf("User added successfully!\n");
                }
                else
                {
                    printf("Error adding user\n");
                }
                break;
                /* This is the case for the search command. It sends the command to the server and then reads the
                       response. The response is a string of users separated by commas. The string is then tokenized
                       and printed. */
            case 2:
                printf("Enter a username to search for: ");
                scanf("%s", username);

                /* Creating a string that will be sent to the server.
                 *  The string will be in the format of "SEARCH username".
                 */
                sprintf(command, "SEARCH %s", username);
                write(sockfd, command, MAX_SIZE);
                n = read(sockfd, response, MAX_SIZE);
                if (n < 0)
                {
                    perror("ERROR reading from socket");
                    exit(1);
                }
                if (strcmp(response, "FOUND") == 0)
                {
                    printf("User found\n");
                    // print the user's information
                    printf("User ID: %s: Username: %s:\n", token, token);
                }
                else
                {
                    printf("User not found\n");
                }
                break;
                /* This is the case for the delete command. It sends the command to the server and then reads the
                       response. The response is a string of users separated by commas. The string is then tokenized
                       and printed. */
            case 3:
                printf("Enter a username to delete: ");
                scanf("%s", username);
                sprintf(command, "DELETE %s", username);
                write(sockfd, command, MAX_SIZE);
                n = read(sockfd, response, MAX_SIZE);
                if (n < 0)
                {
                    perror("ERROR reading from socket");
                    exit(1);
                }
                if (strcmp(response, "OK") == 0)
                {
                    printf("User deleted successfully!\n");
                }
                else
                {
                    printf("Error deleting user\n");
                }
                break;
                /* This is the case for the list command. It sends the command to the server and then reads the
                response. The response is a string of users separated by commas. The string is then tokenized
                and printed. */
            case 4:
                strcpy(command, "LIST");
                write(sockfd, command, MAX_SIZE);
                n = read(sockfd, userlist, MAX_SIZE);
                if (n < 0)
                {
                    perror("ERROR reading from socket");
                    exit(1);
                }
                pch = strtok(userlist, ",");
                while (pch != NULL)
                {
                    printf("%s\n", pch);
                    pch = strtok(NULL, ",");
                }
                break;
            case 5:
                close(sockfd);
                return 0;
                break;
            default:
                printf("Invalid choice\n");
        }
    }
}

// usage: ./client
