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
#include <pthread.h>

#define MAX_SIZE 1024
#define PORT_NUMBER 5678
#define MAX_USERS 100

int userCount = 0;
int userList[MAX_USERS];

/**
 * It reads the accounts.txt file and stores the user IDs in the userList array
 *
 * @return the number of users in the userList array.
 */
void init()
{
    FILE *fp = fopen("accounts.txt", "r");
    char buffer[MAX_SIZE];
    char *token;
    char *pch;

    if (fp == NULL)
    {
        return;
    }

    while (fgets(buffer, sizeof(buffer), fp))
    {
        pch = strtok(buffer, ",");
        while (pch != NULL)
        {
            userList[userCount] = atoi(pch);
            userCount++;
            pch = strtok(NULL, ",");
        }
    }
    fclose(fp);
}

/**
 * It adds a user to the accounts.txt file.
 *
 * @param username The username of the user to be added.
 * @param password The password of the user
 *
 * @return The userid of the user that was added.
 */
int addUser(char *username, char *password)
{
    FILE *fp = fopen("accounts.txt", "a");
    int userid;
    char buffer[MAX_SIZE];

    if (fp == NULL)
    {
        return -1;
    }

    /* Adding the userid to the accounts.txt file. */
    userid = userCount + 1;
    if (userCount == 0)
    {
        fprintf(fp, "%d", userid);
    }
    else
    {
        fprintf(fp, ",%d", userid);
    }
    fclose(fp);

    /* Adding a new user to the accounts.txt file. */
    fp = fopen("accounts.txt", "a");
    fprintf(fp, "\n%d,%s,%s", userid, username, password);


    /* Incrementing the userCount variable by 1. */
    userCount++;


    /* Adding the userid to the userList array. */
    userList[userCount - 1] = userid;
    fclose(fp);
    return userid;
}

/**
 * It reads the accounts.txt file and searches for the username. If it finds it, it returns the userid.
 * If it doesn't find it, it returns -1
 *
 * @param username The username of the user you want to search for.
 *
 * @return The userid of the user if the user exists, -1 otherwise.
 */
int searchUser(char *username)
{
    FILE *fp = fopen("accounts.txt", "r");
    char buffer[MAX_SIZE];
    char *token;
    char *pch;
    int userid;
    int i;

    if (fp == NULL)
    {
        return -1;
    }

    while (fgets(buffer, sizeof(buffer), fp))
    {
        token = strtok(buffer, ",");
        userid = atoi(token);
        token = strtok(NULL, ",");
        if (token != NULL)
        {
            if (strcmp(token, username) == 0)
            {
                fclose(fp);
                return userid;
            }
        }
    }
    fclose(fp);
    return -1;
}

/**
 * It opens the accounts.txt file, reads each line, and if the username matches the username passed in,
 * it deletes the line from the file
 *
 * @param username The username of the user to be deleted.
 *
 * @return The userid of the user that was deleted.
 */
int deleteUser(char *username)
{
    FILE *fp = fopen("accounts.txt", "r");
    FILE *temp = fopen("temp.txt", "w");
    char buffer[MAX_SIZE];
    char *token;
    char *pch;
    int userid;
    int i;
    int found = -1;

    if (fp == NULL)
    {
        return -1;
    }

    while (fgets(buffer, sizeof(buffer), fp))
    {
        token = strtok(buffer, ",");
        userid = atoi(token);
        token = strtok(NULL, ",");
        if (token != NULL)
        {
            if (strcmp(token, username) == 0)
            {
                found = userid;
            }
            else
            {
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

/**
 * It reads the accounts.txt file and stores the user IDs in the userList array
 *
 * @param userlist a string that will contain the list of users
 *
 * @return The number of users in the file.
 */
int listUsers(char *userlist)
{
    FILE *fp = fopen("accounts.txt", "r");
    char buffer[MAX_SIZE];
    char *token;
    char *pch;
    int i;
    int userid;

    if (fp == NULL)
    {
        return -1;
    }

    userCount = 0;
    while (fgets(buffer, sizeof(buffer), fp))
    {
        userCount++;
        token = strtok(buffer, ",");
        userid = atoi(token);
        userList[userCount - 1] = userid;
    }

    for (i = 0; i < userCount; i++)
    {
        if (i == 0)
        {
            sprintf(userlist, "%d", userList[i]);
        }
        else
        {
            sprintf(userlist, "%s,%d", userlist, userList[i]);
        }
    }

    fclose(fp);
    return userCount;
}