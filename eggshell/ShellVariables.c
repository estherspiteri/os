#include <string.h>
#include "ShellVariables.h"

#define SPACING 20

void createVariable(int *num, var **ptr, char* name, char* value);
void initialiseName(int num, var **ptr, char* name);
void initialiseValue(int num, var **ptr, char* value);
void mallocFail(int num, var **ptr);
void mallocFail_NAME(int num, var **ptr);
void mallocFail_VALUE(int num, var **ptr);
void freeArray(int num, var **ptr);

void initVariables(int *num, var **ptr){
    //setting array pointer to NULL and counter to zero
    *ptr = NULL;
    *num = 0;
}

void createVariable(int *num, var **ptr, char* name, char* value){
    *ptr = (var *)realloc(*ptr, sizeof(var)*(*num+1)); //allocates memory for 1 var
    mallocFail(*num, ptr);

    initialiseName(*num, ptr, name);
    initialiseValue(*num, ptr, value);
    (*num)++;
}

void setInitVariables(int *num, var **ptr) {
    createVariable(num, ptr, "PROMPT", "eggshell > ");

    char cwd[100];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        createVariable(num, ptr, "CWD", cwd);
    } else {
        perror("getcwd() error");
        freeArray(*num, ptr);
    }

    char *tty = ttyname(STDIN_FILENO);
    if (tty != NULL) {
        createVariable(num, ptr, "TERMINAL", tty);
    } else {
        perror("ttyname() error");
        freeArray(*num, ptr);
    }
}

char * getVariable(int num, var **ptr, char * name){
    for (int j = 0; j < num; ++j) {
        if (strcmp(name, (*ptr + j)->name) == 0) { //compares var names
            //returns pointer to var which matches inputted name
            return (*ptr + j)->value;
        }
    }
    return NULL;
}

void setVariable(int *num, var **ptr, char *name, char *value) {
    for (int j = 0; j < *num; ++j) {
        if (strcmp(name, (*ptr + j)->name) == 0) { //compares tuple ids
            (*ptr + j)->value = value;
            return;
        }
    }
    createVariable(num, ptr, name, value);
}

void initialiseName(int num, var **ptr, char* name){
    (*ptr+num)->name = (char *)malloc(strlen(name)+1); //allocates memory for var name
    mallocFail_NAME(num, ptr);
    strcpy((*ptr+num)->name, name);                    //copies string id input into var's name
}

void initialiseValue(int num, var **ptr, char* value){
    (*ptr+num)->value = (char *)malloc(strlen(value)+1); //allocates memory for var value
    mallocFail_VALUE(num, ptr);
    strcpy((*ptr+num)->value, value);                    //copies string id input into var's value
}

void displayVariables(int num, var **ptr){
    char temp[50];

    for (int i = 0; i < num; ++i) {
        strcpy(temp, (*ptr+i)->name);
        strcat(temp, " = ");
        strcat(temp, (*ptr+i)->value);

        puts(temp);
    }
}

void freeArray(int num, var **ptr){
    for (int i = 0; i < num; ++i) {
        free((*ptr+num)->name);
        free((*ptr+num)->value);
    }
    free(ptr);
    exit(EXIT_FAILURE);
}

void mallocFail(int num, var **ptr){
    if (*ptr == NULL){ //if memory request fails
        puts("-- Memory allocation failed. --\nGoodbye");
        freeArray(num, ptr);
    }
}

void mallocFail_NAME(int num, var **ptr){
    if ((*ptr+num)->name == NULL){ //if memory request fails
        puts("-- Memory allocation failed. --\nGoodbye");
        freeArray(num, ptr);
    }
}

void mallocFail_VALUE(int num, var **ptr){
    if ((*ptr+num)->value == NULL){ //if memory request fails
        puts("-- Memory allocation failed. --\nGoodbye");
        free((*ptr+num)->name);
        freeArray(num, ptr);
    }
}

