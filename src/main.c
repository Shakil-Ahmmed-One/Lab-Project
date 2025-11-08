#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "students/student.h"
#include "students/result.h"
#include "auth/Authentication System.h"
#include "menu/menu.h"

int main()
{
    AppState state = STATE_AUTH;

    while (state != STATE_EXIT)
    {
        switch (state)
        {
        case STATE_AUTH:
        {
            printf("\nWelcome to the Student Management System for KUET\n");
            printf("-------------------------------------------------\n");
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
            printf("3. Log Out\n");
            printf("4. Exit Program\n");

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
                state = STATE_AUTH;
                break;
            case 4:
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

        default:
            state = STATE_EXIT;
            break;
        }
    }

    printf("\nExiting Student Management System...\n");
    return 0;
}
