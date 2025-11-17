#include <stdio.h>
#include <menu/menu.h>
#include <errno.h>
#include <string.h>
#include "globals.h"
#include "students/student.h"
#include "courses/course.h"

#define MAX_LEN 50

void viewFileContent(FILE *fp)
{
    char ch;
    printf("\n--- File Content ---\n");
    rewind(fp);
    while ((ch = fgetc(fp)) != EOF)
        putchar(ch);
    printf("\n");
}

double calculateCGPA(Student *student)
{

    int status;

    status = MKDIR("data", 0777);
    if (status == -1 && errno != EEXIST)
    {
        perror("Accessing data failed");
        return STATE_MAIN_MENU;
    }

    status = MKDIR("data/results", 0777);
    if (status == -1 && errno != EEXIST)
    {
        perror("Accessing data/results failed");
        return STATE_MAIN_MENU;
    }

    char foldername[30];
    sprintf(foldername, "data/results/%u", student->roll);
    status = MKDIR(foldername, 0777);
    if (status == -1 && errno != EEXIST)
    {
        perror("Accessing data/results failed");
        return STATE_MAIN_MENU;
    }

    char termFilePath[sizeof(foldername) + 10];
    sprintf(termFilePath, "%s/term.txt", foldername);

    char ctFilePath[sizeof(foldername) + 10];
    sprintf(ctFilePath, "%s/ct.txt", foldername);

    FILE *fpCT = fopen(ctFilePath, "a+");
    FILE *fpTerm = fopen(termFilePath, "a+");

    if (fpCT == NULL || fpTerm == NULL)
    {
        perror("Failed to open result files for CGPA calculation");
        return 0.0;
    }

    char filePath[100];
    snprintf(filePath, sizeof(filePath), "data/courses/%u-%u.txt", student->year, student->term);
    FILE *fp = fopen(filePath, "a+");
    if (fp == NULL)
    {
        perror("Failed to open courses file for CGPA calculation");
        return 0.0;
    }

    double creditCount = 0;
    Course course;

    double totalCG = 0;
    while (fscanf(fp, "\n%s %lf", course.name, &course.credit) == 2)
    {
        creditCount += course.credit;
        char courseName[100];
        double totalScore = course.credit * 100;

        int ctNumber;
        double ctScore;

        double totalCTNumber = 0;
        while (fscanf(fpCT, "\n%s -> CT %u: %lf", courseName, &ctNumber, &ctScore) == 3)
        {
            if (strcmp(course.name, courseName) == 0)
            {
                totalCTNumber += ctScore;
            }
        }

        double termScore;

        double totalTermNumber = 0;
        while (fscanf(fpTerm, "\n%s -> Term: %lf", courseName, &termScore) == 2)
        {
            if (strcmp(course.name, courseName) == 0)
            {
                totalTermNumber += termScore;
            }
        }

        double gradePoint = (double)(totalTermNumber + totalCTNumber + 90) * 100 / totalScore; // TODO: 90 is hardcoded
        double g;
        if (gradePoint >= 80)
            g = 4.0;
        else if (gradePoint >= 75)
            g = 3.75;
        else if (gradePoint >= 70)
            g = 3.50;
        else if (gradePoint >= 65)
            g = 3.25;
        else if (gradePoint >= 60)
            g = 3.0;
        else if (gradePoint >= 55)
            g = 2.75;
        else if (gradePoint >= 50)
            g = 2.5;
        else if (gradePoint >= 45)
            g = 2.25;
        else if (gradePoint >= 40)
            g = 2.0;
        else
            g = 0.0;

        totalCG += g * course.credit;
    }

    double cgpa = totalCG / creditCount;

    fclose(fp);
    fclose(fpCT);
    fclose(fpTerm);

    return cgpa;
}

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

        int status;

        status = MKDIR("data", 0777);
        if (status == -1 && errno != EEXIST)
        {
            perror("Accessing data failed");
            return STATE_MAIN_MENU;
        }

        status = MKDIR("data/results", 0777);
        if (status == -1 && errno != EEXIST)
        {
            perror("Accessing data/results failed");
            return STATE_MAIN_MENU;
        }

        char foldername[30];
        sprintf(foldername, "data/results/%u", roll);
        status = MKDIR(foldername, 0777);
        if (status == -1 && errno != EEXIST)
        {
            perror("Accessing data/results failed");
            return STATE_MAIN_MENU;
        }

        Student student = findStudentByRoll(roll);

        if (student.roll == 0)
        {
            printf("No student found with roll %u.\n", roll);
            return STATE_MAIN_MENU;
            return STATE_MAIN_MENU;
        }

        printf("\nResult of Roll %u\n", roll);
        printf("----------------------\n");
        printf("Name: %s\n", student.name);
        printf("Approximate CGPA: %.2lf\n", calculateCGPA(&student));
        printf("----------------------\n");
        printf("1. CT\n");
        printf("2. Term\n");
        printf("3. Overall\n");
        printf("4. Go Back\n");
        printf("5. Return to main menu\n");

        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar();

        FILE *fp = NULL;

        char termFilePath[sizeof(foldername) + 10];
        sprintf(termFilePath, "%s/term.txt", foldername);

        char ctFilePath[sizeof(foldername) + 10];
        sprintf(ctFilePath, "%s/ct.txt", foldername);

        switch (choice)
        {
        case 1:
        {
            fp = fopen(ctFilePath, "a+");
            if (fp == NULL)
            {
                perror("Failed to open ct.txt");
                return STATE_MAIN_MENU;
            }

            printf("\nCT Results\n");
            printf("----------\n");
            printf("1. View CT Results\n");
            printf("2. Add CT Result\n");
            printf("3. Go Back\n");
            printf("Enter your choice: ");

            int ctChoice;
            scanf("%d", &ctChoice);
            getchar();

            switch (ctChoice)
            {
            case 1:
            {
                viewFileContent(fp);
                break;
            }
            case 2:
            {
                char course[20];
                printf("Enter Course Name: ");
                fgets(course, sizeof(course), stdin);
                course[strcspn(course, "\n")] = '\0';

                unsigned int ctNumber;
                printf("Enter CT Number: ");
                scanf("%u", &ctNumber);
                getchar();

                double ctScore;
                printf("Enter CT Score: ");
                scanf("%lf", &ctScore);
                getchar();

                fprintf(fp, "%s -> CT %u: %.2f\n", course, ctNumber, ctScore);
                printf("CT Result added successfully.\n");
                break;
            }

            default:
                break;
            }

            fclose(fp);
            break;
        }
        case 2:
        {
            fp = fopen(termFilePath, "a+");
            if (fp == NULL)
            {
                perror("Failed to open term.txt");
                return STATE_MAIN_MENU;
            }

            printf("Term Results\n");
            printf("------------\n");
            printf("1. View Term Results\n");
            printf("2. Add Term Result\n");
            printf("3. Go Back\n");
            printf("Enter your choice: ");

            int termChoice;
            scanf("%d", &termChoice);
            getchar();

            switch (termChoice)
            {
            case 1:
            {
                viewFileContent(fp);
                break;
            }
            case 2:
            {
                char course[20];
                printf("Enter Course Name: ");
                fgets(course, sizeof(course), stdin);
                course[strcspn(course, "\n")] = '\0';

                double termScore;
                printf("Enter Term Score: ");
                scanf("%lf", &termScore);
                getchar();

                fprintf(fp, "%s Term Score: %.2f\n", course, termScore);
                printf("Term Result added successfully.\n");
                break;
            }
            case 3:
                return STATE_MAIN_MENU;
                break;
            default:
                break;
            }
            if (fp != NULL)
            {
                fclose(fp);
            }
            break;
        }
        case 3:
        {
            printf("Overall Results\n");
            printf("---------------\n");
            fp = fopen(ctFilePath, "a+");
            if (fp == NULL)
            {
                perror("Failed to open ct.txt");
                return STATE_MAIN_MENU;
            }
            viewFileContent(fp);
            printf("\n");
            fclose(fp);
            fp = fopen(termFilePath, "a+");
            if (fp == NULL)
            {
                perror("Failed to open term.txt");
                return STATE_MAIN_MENU;
            }
            viewFileContent(fp);
            fclose(fp);
            break;
        }
        case 4:
        {
            return STATE_RESULT_MENU;
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
    }
}