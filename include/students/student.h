#ifndef STUDENT_H
#define STUDENT_H

#include "menu/menu.h"

typedef struct
{
    char name[100];
    unsigned int roll;
    unsigned int year;
    unsigned int term;
} Student;

Student findStudentByRoll(unsigned int);
AppState manageStudents();

#endif // STUDENT_H
