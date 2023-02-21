#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "main.h"

#define BUFFER_SIZE 512
char *arguments[50];
char buffer[BUFFER_SIZE], *token;
int main(int argc, char **argv) {
    loop_shell();
    return 0;
}

void loop_shell() {
    do{
        display();
        read_parse();
        start_fork();
        if(feof(stdin)){
            break;
        }

    } while(strcmp(buffer, "quit") != 0 );
}

void display(){
    char cwd[256];
    getcwd(cwd, 256);
    printf("%s>>> ", cwd);
}


void read_parse(){
    char delim[] = " \n\t()<>|&;"; // Each token to be split by whitespace 
    char *c;
    char *t;
        t = fgets(buffer, BUFFER_SIZE, stdin);
        if(buffer[strlen(buffer) - 1] != '\n' && !feof(stdin)) {
            char c;
            while(c = getchar() != '\n' && c != EOF);
            printf("input should not exceed 512 characters, try again\n");
        }
        else {
            int i = 0;
            char *token = strtok(buffer, delim);
            while(token != NULL && i < 49) {
                arguments[i] = malloc(strlen(token));
                strcpy(arguments[i], token);
                token = strtok(NULL, delim);
                i++;
            }
            arguments[i] = NULL;
        } 
    }

int start_fork() {
    pid_t pid;
    pid = fork();
    if(pid < 0) {
        fprintf(stderr, "Fork Failed");
        return 1;
    }
    else if (pid == 0) { /* Child Process */  
        if (execvp(arguments[0], arguments) < 0) {
            if((strcmp(arguments[0], "quit"))) {
                perror(arguments[0]);
            }     /* execute the command  */
            exit(1);
        }
    }
    else { /* parent process */
        /* parent will wait for the child process to complete*/
        wait(NULL);
        return 0;
    }
}




    

