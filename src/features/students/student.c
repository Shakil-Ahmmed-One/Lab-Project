#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>

#include "students/student.h"
#include "menu/menu.h"
#include "globals.h"

void addStudent()
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

    printf("Enter Student Year: ");
    unsigned int year;
    scanf("%u", &year);
    getchar();

    printf("Enter Student Term: ");
    unsigned int term;
    scanf("%u", &term);
    getchar();

    FILE *fp = fopen("data/students/students.txt", "a+");
    if (fp == NULL)
    {
        perror("Failed to open students.txt");
    }

    bool found = false;
    Student student;
    while (fscanf(fp, "\n%99[^0-9] %u %u %u", student.name, &student.roll, &student.year, &student.term) == 4 && fp != NULL)
    {
        if (student.roll == roll)
        {
            found = true;
            printf("\nStudent is already added in the database.\n");
            printf("Name: %s\n", student.name);
            printf("Roll: %u\n", student.roll);
            printf("Session: %u-%u\n", student.year, student.term);
            break;
        }
    }
    if (!found)
    {
        fprintf(fp, "%s %u %u %u\n", name, roll, year, term);
        printf("Student added successfully!\n");
    }

    fclose(fp);
}

Student findStudentByRoll(unsigned int roll)
{
    Student student;
    FILE *fp = fopen("data/students/students.txt", "a+");
    if (fp == NULL)
    {
        perror("Failed to open students.txt");
    }

    bool found = false;
    while (fscanf(fp, "\n%99[^0-9] %u %u %u", student.name, &student.roll, &student.year, &student.term) == 4 && fp != NULL)
    {
        if (student.roll == roll)
        {
            found = true;
            break;
        }
    }
    if (!found)
    {
        student.roll = 0; // Indicate not found
    }
    fclose(fp);
    return student;
}

void displayStudent()
{
    printf("\nDisplay Student\n");
    printf("-------------\n");
    printf("Enter Student Roll: ");
    unsigned int roll;
    scanf("%u", &roll);

    Student student = findStudentByRoll(roll);
    if (student.roll == 0)
    {
        printf("No student was found!\n");
    }
    else
    {
        printf("\nStudent found:\n");
        printf("Name: %s\n", student.name);
        printf("Roll: %u\n", student.roll);
        printf("Session: %u-%u\n", student.year, student.term);
    }
}

void deleteStudent()
{
    printf("\nDelete Student\n");
    printf("-------------\n");
    printf("Enter student roll: ");
    unsigned int roll;
    scanf("%u", &roll);
    FILE *fp = fopen("data/students/students.txt", "a+");
    if (fp == NULL)
    {
        perror("Failed to open students.txt");
    }
    FILE *fp2 = fopen("data/students/temp.txt", "w");

    bool found = false;
    Student student;
    while (fscanf(fp, "\n%99[^0-9] %u", student.name, &student.roll) == 2 && fp != NULL)
    {
        if (student.roll != roll)
        {
            fprintf(fp2, "%s %u\n", student.name, student.roll);
        }
        else
        {
            found = true;
        }
    }
    fclose(fp2);
    fclose(fp);
    if (remove("data/students/students.txt") != 0 || rename("data/students/temp.txt", "data/students/students.txt") != 0)
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
    while (1)
    {
        int choice;
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

        int status;

        status = MKDIR("data", 0777);
        if (status == -1 && errno != EEXIST)
        {
            perror("Accessing data failed");
            return STATE_MAIN_MENU;
        }

        status = MKDIR("data/students", 0777);
        if (status == -1 && errno != EEXIST)
        {
            perror("Accessing data/students failed");
            return STATE_MAIN_MENU;
        }

        switch (choice)
        {
        case 1:
        {
            addStudent();
            break;
        }
        case 2:
        {
            displayStudent();
            break;
        }
        case 3:
        {
            deleteStudent();
            break;
        }
        case 4:
        case 5:
        {
            return STATE_MAIN_MENU;
            break;
        }
        default:
        {
            printf("Invalid choice!\n");
            break;
        }
        }
    }
}
