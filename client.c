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
    int sockfd;
    int n;
    int i;
    int choice;
    int userid;
    int numusers;
    int found;
    struct sockaddr_in serv_addr;
    char buffer[MAX_SIZE];
    char command[MAX_SIZE];
    char response[MAX_SIZE];
    char username[MAX_SIZE];
    char password[MAX_SIZE];
    char userlist[MAX_SIZE];
    char *token;
    char *pch;

    Queue q;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("ERROR opening socket");
        exit(1);
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT_NUMBER);
    serv_addr.sin_addr.s_addr = inet_addr(SERVER_ADDR);
    if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0)
    {
        perror("ERROR connecting");
        exit(1);
    }

    printf("Connected to server\n");

    initQueue(&q);

    while (1) {
        printf("\nEnter a command:\n");
        printf("1. Add User\n");
        printf("2. Search Users\n");
        printf("3. Delete Users\n");
        printf("4. List Users\n");
        printf("5. Exit\n");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter a username: ");
                scanf("%s", username);
                printf("Enter a password: ");
                scanf("%s", password);
                sprintf(command, "ADD %s %s", username, password);
                write(sockfd, command, MAX_SIZE);
                n = read(sockfd, response, MAX_SIZE);
                if (n < 0) {
                    perror("ERROR reading from socket");
                    exit(1);
                }
                if (strcmp(response, "OK") == 0) {
                    printf("User added successfully!\n");
                } else {
                    printf("Error adding user\n");
                }
                break;
            case 2:
                printf("Enter a username to search for: ");
                scanf("%s", username);
                sprintf(command, "SEARCH %s", username);
                write(sockfd, command, MAX_SIZE);
                n = read(sockfd, response, MAX_SIZE);
                if (n < 0) {
                    perror("ERROR reading from socket");
                    exit(1);
                }
                if (strcmp(response, "FOUND") == 0) {
                    printf("User found\n");
                } else {
                    printf("User not found\n");
                }
                break;
            case 3:
                printf("Enter a username to delete: ");
                scanf("%s", username);
                sprintf(command, "DELETE %s", username);
                write(sockfd, command, MAX_SIZE);
                n = read(sockfd, response, MAX_SIZE);
                if (n < 0) {
                    perror("ERROR reading from socket");
                    exit(1);
                }
                if (strcmp(response, "OK") == 0) {
                    printf("User deleted successfully!\n");
                } else {
                    printf("Error deleting user\n");
                }
                break;
            case 4:
                strcpy(command, "LIST");
                write(sockfd, command, MAX_SIZE);
                n = read(sockfd, userlist, MAX_SIZE);
                if (n < 0) {
                    perror("ERROR reading from socket");
                    exit(1);
                }
                pch = strtok(userlist,",");
                while (pch != NULL)
                {
                    printf("%s\n",pch);
                    pch = strtok (NULL, ",");
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

