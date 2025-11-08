#ifndef AUTHENTICATION_SYSTEM_H_INCLUDED
#define AUTHENTICATION_SYSTEM_H_INCLUDED

#include <stdbool.h>

typedef struct
{
    char username[21];
    char password[65];
} User;

extern User user;

bool signIn();
bool signUp();
bool auth();

#endif // AUTHENTICATION_SYSTEM_H_INCLUDED
