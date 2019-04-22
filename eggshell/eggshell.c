#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include "linenoise/linenoise.c"
#include "linenoise/linenoise.h"
#include "ShellVariables.c"
#include "ShellVariables.h"
#include "stdbool.h"

int main(void) {
    int varCount;
    var * variables = NULL;

    initVariables(&varCount, &variables);
    setInitVariables(&varCount, &variables);
    char *line;

    const char delim[2] = " ";
    char *token;

    char *args[100];

    char prompt[PROMPT_VAR];
    int status;
    time_t curtime;
    time(&curtime);

    while((line = linenoise(getVariable(varCount, &variables, "PROMPT"))) != NULL) {
        if (strcmp(line, "exit") == 0){
            free(line);
            exit(0);
        }
        else if (strcmp(line, "env") == 0){
            displayVariables(varCount, &variables);
        }
        else {

            int j = 0;

            /* get the first token */
            token = strtok(line, delim);

            while (token != NULL) {

                args[j] = token;

                token = strtok(NULL, delim);
                j++;
            }
            args[j] = (char *) NULL;

            if (strcmp(args[0], "setenv") == 0) {
                if (args[1] != NULL && args[2] != NULL) {
                    setVariable(&varCount, &variables, args[1], args[2]);
                }
                else{
                    puts("-->  setenv \"name\" \"value\"  <--");
                }
            }
            else {
                pid_t pid = fork();

                if (pid == -1) {
                    printf("%d\n", errno);
                    perror("fork() failed");
                    exit(EXIT_FAILURE);
                } else if (pid == 0) {
                    if (execvp(args[0], args)) {
                        perror("execvp() failed");
                        exit(EXIT_FAILURE);
                    }

                    //dead code
                    printf("This string should never get printed\n");

                    //terminate child
                    exit(EXIT_SUCCESS);
                } else {
                    do {
                        pid = waitpid(pid, &status, WNOHANG); //wait until a state change in the child process
                        //WNOHSNG --> checks child processes without causing the caller to be suspended

                        if (pid > 0) {
                            if (WIFEXITED(status) > 0) {
                                //returned from main(), or else called the exit() or _exit() function
                                printf("Child exited with status of %d\n", WEXITSTATUS(status));
                            } else {
                                printf("Child did not exit successfully\n");
                                printf("Child is still running at %s", ctime(&curtime));
                                //ctime() --> returns a string representing the localtime based on the argument timer.
                                sleep(1);
                            }
                        }
//                else if (pid == 0) {
//                    printf("Status information is not available!\n");
//                }
                        else if (pid == -1) {
                            printf("%d\n", errno);
                            perror("Wait failed!");
                            exit(EXIT_FAILURE);
                        }
                    } while (pid == 0);


                    if (WEXITSTATUS(status) != EXIT_FAILURE) {
                        memset(prompt, 0, sizeof(prompt));
                        strcat(prompt, args[0]);
                        strcat(prompt, " > ");
                        setVariable(&varCount, &variables, "PROMPT", prompt);
                    }
                }
            }
        }
        free(line);
    }
    return 0;
}

