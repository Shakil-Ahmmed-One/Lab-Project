#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "students/student.h"
#include "courses/course.h"
#include "results/result.h"
#include "auth/AuthenticationSystem.h"
#include "menu/menu.h"

int main(int argc, char *argv[])
{
    AppState state;
    User user1;
    if (argc >= 3)
    {
        strncpy(user1.username, argv[1],strlen(argv[1]));
        user1.username[strlen(argv[1])] = '\0';
        strncpy(user1.password, argv[2],strlen(argv[2]));
        user1.password[strlen(argv[2])] = '\0';
        FILE *fp = fopen("data/users/users.dat", "r");
        if (fp == NULL)
        {
            perror("Failed to open users.dat");
            state=STATE_AUTH;
            fclose(fp);
        }
        while (!feof(fp))
        {
            if (fread(&user, sizeof(User), 1, fp))
            {
                if (strcmp(user1.username, user.username) == 0)
                {
                    state= STATE_MAIN_MENU;
                    fclose(fp);
                    break;
                }
            }
            else
            {
                fclose(fp);
                break;
            }
        }
    }
    else
    {
        state = STATE_AUTH;
    }
    while (state != STATE_EXIT)
    {
        switch (state)
        {
        case STATE_AUTH:
        {
            printf("\nWelcome to the Student Management System for KUET\n");
            printf("---------------------------------------------------\n");
            bool isAuthenticated = auth();
            if (isAuthenticated)
                state = STATE_MAIN_MENU;
            else
                state = STATE_EXIT;
            break;
        }
        case STATE_MAIN_MENU:
        {
            printf("\nWelcome, %s!\n", user.username);
            printf("-----------------------------\n");
            printf("1. Manage Students\n");
            printf("2. Manage Results\n");
            printf("3. Manage Courses\n");
            printf("4. Log Out\n");
            printf("5. Exit Program\n");

            int choice;
            printf("Enter your choice: ");
            scanf("%d", &choice);
            getchar();
            switch (choice)
            {
            case 1:
                state = STATE_STUDENT_MENU;
                break;
            case 2:
                state = STATE_RESULT_MENU;
                break;
            case 3:
                state = STATE_COURSE_MENU;
                break;
            case 4:
                state = STATE_AUTH;
                break;
            case 5:
                state = STATE_EXIT;
                break;
            default:
                printf("Invalid choice!\n");
                break;
            }
            break;
        }

        case STATE_STUDENT_MENU:
        {
            state = manageStudents();
            break;
        }

        case STATE_RESULT_MENU:
        {
            state = manageResults();
            break;
        }

        case STATE_COURSE_MENU:
        {
            state = manageCourses();
            break;
        }

        default:
            state = STATE_EXIT;
            break;
        }
    }

    printf("\nExiting Student Management System...\n");
    return 0;
}
