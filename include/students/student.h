#ifndef STUDENT_H
#define STUDENT_H

#include <stdio.h>
#include "menu/menu.h"

typedef struct
{
    char name[100];
    unsigned int roll;
} Student;

void addStudent(FILE *fp, Student *student);
void displayStudent(FILE *fp, Student *student);
void deleteStudent(FILE *fp, Student *student);
AppState manageStudents();

#endif // STUDENT_H
