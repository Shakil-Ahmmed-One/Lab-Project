#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>
#include <stdlib.h>
#include "auth/Authentication System.h"

#ifdef _WIN32
#include <direct.h>
#define MKDIR(path, mode) _mkdir(path)
#else
#include <sys/stat.h>
#include <sys/types.h>
#define MKDIR(path, mode) mkdir((path), (mode))
#endif

User user;

bool signIn()
{
    printf("\nSign In\n");
    printf("-------\n");
    User stored;
    printf("Enter username: ");
    scanf("%20s", user.username);
    printf("Enter password: ");
    scanf("%64s", user.password);

    FILE *fp = fopen("data/users/users.dat", "a+");

    if (fp == NULL)
    {
        perror("Failed to access users.dat");
        return false;
    }
    while (fread(&stored, sizeof(User), 1, fp))
    {
        if (strcmp(user.username, stored.username) == 0 && strcmp(user.password, stored.password) == 0)
        {
            fclose(fp);
            printf("Access granted!\n");
            return true;
        }
    }
    fclose(fp);
    printf("No Users found with the given credentials!\n");
    return false;
}

bool signUp()
{
    printf("\nSign Up\n");
    printf("-------\n");
    printf("Enter username (without spaces): ");
    scanf("%20s", user.username);
    printf("Enter password: ");
    scanf("%64s", user.password);
    FILE *fp = fopen("data/users/users.dat", "a");
    if (fp == NULL)
    {
        perror("Failed to access users.dat");
        return false;
    }

    fwrite(&user, sizeof(user), 1, fp);
    fclose(fp);
    printf("Signup successful!\n");

    return true;
}

bool auth()
{
    int status;

    status = MKDIR("data", 0777);
    if (status == -1 && errno != EEXIST)
    {
        perror("Accessing data failed");
        return false;
    }

    status = MKDIR("data/users", 0777);
    if (status == -1 && errno != EEXIST)
    {
        perror("Accessing data/users failed");
        return false;
    }

    bool isAuthenticated = false;
    do
    {
        int choice;
        printf("1. Login\n");
        printf("2. Register\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();
        switch (choice)
        {
        case 1:
        {
            isAuthenticated = signIn();
            break;
        }
        case 2:
        {
            isAuthenticated = signUp();
            break;
        }
        case 3:
        {
            exit(0);
            break;
        }
        default:
        {
            printf("Invalid choice!\n");
            break;
        }
        }
    } while (isAuthenticated == false);

    return isAuthenticated;
}