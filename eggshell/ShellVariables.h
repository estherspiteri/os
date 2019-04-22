#ifndef EGGSHELL_SHELLVARIABLES_H
#define EGGSHELL_SHELLVARIABLES_H

//#include "ShellVariables.c"
#include <stdlib.h>
#include <stdio.h>
#define PROMPT_VAR 20

typedef struct variables {
    char *name;
    char *value;
}var;

//shell variables
void initVariables(int *num, var **ptr);
void setInitVariables(int *num, var **ptr);
char * getVariable(int num, var **ptr, char * name);
void setVariable(int *num, var **ptr, char *name, char *value);
void displayVariables(int num, var **ptr);

#endif //EGGSHELL_SHELLVARIABLES_H
