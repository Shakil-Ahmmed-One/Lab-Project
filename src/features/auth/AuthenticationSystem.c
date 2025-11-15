#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>
#include <stdlib.h>
#include <ctype.h>
#include "auth/AuthenticationSystem.h"
#include "globals.h"
#include <conio.h>
User user;
int takepassword(char pass[50])
{
    char ch;
    int i = 0;
    while (1)
    {
        ch = getch();
        if (ch == 32 || ch == 9 || ch == 10 || ch == 11 || ch == 12 || ch == 27)
        {
            continue;
        }
        else if (ch == 13)
        {
            pass[i] = '\0';
            printf("\n");
            break;
        }
        else if (ch == 8)
        {
            if (i >0)
            {
                i--;
                printf("\b \b");
            }
        }
        else
        {
            pass[i] = ch;
            i++;
            printf("*");
        }
    }
}
int isValidPassword(char *password) {
    int len = strlen(password);
    if (len < 8) return 0;
    int hasAlpha = 0, hasDigit = 0;
    for (int i = 0; i < len; i++) {
        if (isalpha(password[i])) hasAlpha = 1;
        else if (isdigit(password[i])) hasDigit = 1;
    }
    return hasAlpha && hasDigit;
}

bool signIn()
{
    printf("\nSign In\n");
    printf("-------\n");
    User stored;
    printf("Enter username: ");
    scanf("%20s", user.username);
    printf("Enter password: ");
    takepassword(user.password);
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
    char confirmPassword[65];
    printf("Enter password: ");
    takepassword(user.password);
    printf("Confirm password: ");
    takepassword(confirmPassword);
    if (strcmp(user.password, confirmPassword) != 0) {
        printf("Passwords do not match!\n");
        return false;
    }
    if(isValidPassword(user.password) == 0) {
        printf("Password must be at least 8 characters long and contain both letters and digits.\n");
        return false;
    }

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