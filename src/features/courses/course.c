#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>

#include "courses/course.h"
#include "menu/menu.h"
#include "globals.h"

void addCourse(char *filePath)
{
    printf("\nAdd Course\n");
    printf("-----------\n");
    char name[100];
    printf("Enter Course Name: ");
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = '\0';

    double credit;
    printf("Enter Course Credit: ");
    scanf("%lf", &credit);
    getchar();

    FILE *fp = fopen(filePath, "a+");
    if (fp == NULL)
    {
        perror("Failed to open courses info");
    }
    Course course;
    bool found = false;
    while (fscanf(fp, "\n%s %lf", course.name, &course.credit) == 2 && fp != NULL)
    {
        if (strcmp(course.name, name) == 0)
        {
            found = true;
            printf("\nCourse is already added in the database.\n");
            printf("Name: %s\n", course.name);
            break;
        }
    }
    if (!found)
    {
        fprintf(fp, "%s %lf\n", name, credit);
        printf("Course added successfully!\n");
    }

    fclose(fp);
}

void allCourses(char *filePath)
{
    printf("\nCourses List\n");
    printf("-------------\n");

    FILE *fp = fopen(filePath, "a+");
    if (fp == NULL)
    {
        perror("Failed to open courses info");
    }
    Course course;
    printf("%-30s %-10s\n", "Course Name", "Credit");
    printf("-----------------------------------------\n");
    while (fscanf(fp, "\n%s %lf", course.name, &course.credit) == 2 && fp != NULL)
    {
        printf("%-30s %-10.2lf\n", course.name, course.credit);
    }
    fclose(fp);
}

void deleteCourse(char *filePath)
{
    printf("\nDelete Course\n");
    printf("--------------\n");
    printf("Enter course name: ");
    char name[20];
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = '\0';

    FILE *fp = fopen(filePath, "a+");
    if (fp == NULL)
    {
        perror("Failed to open courses info");
    }
    FILE *fp2 = fopen("data/courses/temp.txt", "w");
    Course course;
    bool found = false;
    while (fscanf(fp, "\n%s %lf", course.name, &course.credit) == 2 && fp != NULL)
    {
        if (strcmp(course.name, name) != 0)
        {
            fprintf(fp2, "%s %lf\n", course.name, course.credit);
        }
        else
        {
            found = true;
        }
    }
    fclose(fp2);
    fclose(fp);
    if (remove(filePath) != 0 || rename("data/courses/temp.txt", filePath) != 0)
    {
        printf("Failed to delete Course\n");
    }
    else if (!found)
    {
        printf("No course was found!\n");
    }
    else
    {
        printf("Course Deleted Successfully\n");
    }
}

AppState manageCourses()
{
    while (1)
    {
        int choice;
        printf("\nManage Courses\n");
        printf("--------------\n");

        unsigned int year, term;
        printf("Enter Year: ");
        scanf("%u", &year);
        getchar();

        printf("Enter Term: ");
        scanf("%u", &term);
        getchar();

        int status;

        status = MKDIR("data", 0777);
        if (status == -1 && errno != EEXIST)
        {
            perror("Accessing data failed");
            return STATE_MAIN_MENU;
        }

        status = MKDIR("data/courses", 0777);
        if (status == -1 && errno != EEXIST)
        {
            perror("Accessing data/courses failed");
            return STATE_MAIN_MENU;
        }

        char filePath[64];
        snprintf(filePath, sizeof(filePath), "data/courses/%u-%u.txt", year, term);
        FILE *fp = fopen(filePath, "a+");
        if (fp == NULL)
        {
            perror("Accessing courses data failed");
            return STATE_MAIN_MENU;
        }

        printf("Manage courses of %u-%u\n", year, term);
        printf("-----------------------\n");
        printf("1. Add Course\n");
        printf("2. Courses List\n");
        printf("3. Delete Course\n");
        printf("4. Back\n");
        printf("5. Return to Main Menu\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();

        switch (choice)
        {
        case 1:
        {
            addCourse(filePath);
            break;
        }
        case 2:
        {
            allCourses(filePath);
            break;
        }
        case 3:
        {
            deleteCourse(filePath);
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
