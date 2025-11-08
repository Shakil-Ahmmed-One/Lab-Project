#include <stdio.h>
#include <menu/menu.h>

AppState manageResults()
{
    int choice;
    while (1)
    {

        printf("\nManage Results\n");
        printf("--------------\n");
        unsigned int roll;
        printf("Enter Student Roll to view results: ");
        scanf("%u", &roll);
        getchar();
        FILE *fp = fopen("students.txt", "r");
        if (fp == NULL)
        {
            perror("Failed to open students.txt");
            return STATE_MAIN_MENU;
        }

        printf("1. CT\n");
        printf("2. Term\n");
        printf("3. Overall\n");

        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();

        switch (choice)
        {
        case 1:
        {
            printf("CT Results\n");
            printf("----------\n");
            break;
        }
        case 2:
        {
            printf("Term Results\n");
            printf("------------\n");
            break;
        }
        case 3:
        {
            printf("Overall Results\n");
            printf("---------------\n");
            break;
        }
        case 4:
        {
            break;
        }
        case 5:
        {
            printf("Return to Main Menu\n");
            return STATE_MAIN_MENU;
        }
        default:
        {
            printf("Invalid choice!\n");
            break;
        }
        }

        fclose(fp);
    }
}