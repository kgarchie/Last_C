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

int userCount = 0;
int userList[MAX_USERS];

void init() {
    FILE *fp = fopen("accounts.txt", "r");
    char buffer[MAX_SIZE];
    char *token;
    char *pch;

    if (fp == NULL) {
        return;
    }

    while (fgets(buffer, sizeof(buffer), fp)) {
        pch = strtok (buffer,",");
        while (pch != NULL)
        {
            userList[userCount] = atoi(pch);
            userCount++;
            pch = strtok (NULL, ",");
        }
    }
    fclose(fp);
}

int addUser(char *username, char *password) {
    FILE *fp = fopen("accounts.txt", "a");
    int userid;
    char buffer[MAX_SIZE];

    if (fp == NULL) {
        return -1;
    }

    userid = userCount + 1;
    if (userCount == 0) {
        fprintf(fp, "%d", userid);
    } else {
        fprintf(fp, ",%d", userid);
    }
    fclose(fp);

    fp = fopen("accounts.txt", "a");
    fprintf(fp, "\n%d,%s,%s", userid, username, password);
    userCount++;
    userList[userCount - 1] = userid;
    fclose(fp);
    return userid;
}

int searchUser(char *username) {
    FILE *fp = fopen("accounts.txt", "r");
    char buffer[MAX_SIZE];
    char *token;
    char *pch;
    int userid;
    int i;

    if (fp == NULL) {
        return -1;
    }

    while (fgets(buffer, sizeof(buffer), fp)) {
        token = strtok(buffer, ",");
        userid = atoi(token);
        token = strtok(NULL, ",");
        if (token != NULL) {
            if (strcmp(token, username) == 0) {
                fclose(fp);
                return userid;
            }
        }
    }
    fclose(fp);
    return -1;
}

int deleteUser(char *username) {
    FILE *fp = fopen("accounts.txt", "r");
    FILE *temp = fopen("temp.txt", "w");
    char buffer[MAX_SIZE];
    char *token;
    char *pch;
    int userid;
    int i;
    int found = -1;

    if (fp == NULL) {
        return -1;
    }

    while (fgets(buffer, sizeof(buffer), fp)) {
        token = strtok(buffer, ",");
        userid = atoi(token);
        token = strtok(NULL, ",");
        if (token != NULL) {
            if (strcmp(token, username) == 0) {
                found = userid;
            } else {
                fprintf(temp, "%s\n", buffer);
            }
        }
    }
    fclose(fp);
    fclose(temp);
    remove("accounts.txt");
    rename("temp.txt", "accounts.txt");

    return found;
}

int listUsers(char *userlist) {
    FILE *fp = fopen("accounts.txt", "r");
    char buffer[MAX_SIZE];
    char *token;
    char *pch;
    int i;
    int userid;

    if (fp == NULL) {
        return -1;
    }

    userCount = 0;
    while (fgets(buffer, sizeof(buffer), fp)) {
        userCount++;
        token = strtok(buffer, ",");
        userid = atoi(token);
        userList[userCount - 1] = userid;
    }

    for (i = 0; i < userCount; i++) {
        if (i == 0) {
            sprintf(userlist, "%d", userList[i]);
        } else {
            sprintf(userlist, "%s,%d", userlist, userList[i]);
        }
    }

    fclose(fp);
    return userCount;
}