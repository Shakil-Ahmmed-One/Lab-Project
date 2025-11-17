#ifndef COURSE_H
#define COURSE_H

#include "menu/menu.h"

typedef struct
{
    char name[100];
    double credit;
} Course;

AppState manageCourses();

#endif // COURSE_H
