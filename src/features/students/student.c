#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "students/student.h"
#include "menu/menu.h"

void addStudent(FILE *fp, Student *student)
{
    printf("\nAdd Student\n");
    printf("-------------\n");
    printf("Enter Student Name: ");
    char name[100];
    unsigned int roll;
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = '\0';

    printf("Enter Student Roll: ");
    scanf("%u", &roll);
    getchar();

    bool found = false;
    while (fscanf(fp, "\n%99[^0-9] %u", student->name, &student->roll) == 2 && fp != NULL)
    {
        if (student->roll == roll)
        {
            found = true;
            printf("\nStudent is already added in the database.\n");
            printf("Name: %s\n", student->name);
            printf("Roll: %u\n", student->roll);
            break;
        }
    }
    if (!found)
    {
        fprintf(fp, "%s %u\n", student->name, student->roll);
        printf("Student added successfully!\n");
    }
}

void displayStudent(FILE *fp, Student *student)
{
    printf("\nDisplay Student\n");
    printf("-------------\n");
    printf("Enter Student Roll: ");
    unsigned int roll;
    scanf("%u", &roll);
    getchar();
    rewind(fp);

    bool found = false;
    while (fscanf(fp, "\n%99[^0-9] %u", student->name, &student->roll) == 2 && fp != NULL)
    {
        if (student->roll == roll)
        {
            found = true;
            printf("\nStudent found:\n");
            printf("Name: %s\n", student->name);
            printf("Roll: %u\n", student->roll);
            break;
        }
    }
    if (!found)
    {
        printf("No student was found!\n");
    }
}

void deleteStudent(FILE *fp, Student *student)
{
    printf("\nDelete Student\n");
    printf("-------------\n");
    printf("Enter student roll: ");
    unsigned int roll;
    scanf("%u", &roll);
    rewind(fp);
    FILE *fp2 = fopen("temp.txt", "a+");

    bool found = false;
    while (fscanf(fp, "\n%99[^0-9] %u", student->name, &student->roll) == 2 && fp != NULL)
    {
        if (student->roll != roll)
        {
            fprintf(fp2, "%s %u\n", student->name, student->roll);
        }
        else
        {
            found = true;
        }
    }
    if (remove("students.txt") != 0 || rename("temp.txt", "students.txt") != 0)
    {
        printf("Failed to delete Student\n");
    }
    else if (!found)
    {
        printf("No student was found!\n");
    }
    else
    {
        printf("Student Deleted Successfully\n");
    }
}

AppState manageStudents()
{
    FILE *fp = fopen("students.txt", "a+");
    if (fp == NULL)
    {
        perror("Failed to open students.txt");
        return STATE_MAIN_MENU;
    }

    Student student;
    int choice;

    while (1)
    {
        printf("\nManage Students\n");
        printf("---------------\n");
        printf("1. Add Student\n");
        printf("2. Display Student\n");
        printf("3. Delete Student\n");
        printf("4. Back\n");
        printf("5. Return to Main Menu\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();

        switch (choice)
        {
        case 1:
        {
            addStudent(fp, &student);
            break;
        }
        case 2:
        {
            displayStudent(fp, &student);
            break;
        }
        case 3:
        {
            deleteStudent(fp, &student);
            break;
        }
        case 4:
        {
            break;
        }
        case 5:
        {
            fclose(fp);
            return STATE_MAIN_MENU;
        }
        default:
        {
            printf("Invalid choice!\n");
            break;
        }
        }
    }

    fclose(fp);
}
