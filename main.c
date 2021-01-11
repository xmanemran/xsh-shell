#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LINE 80

void print(char*);
void parse_cmd(char *cmd, char *args[], int* arg_length);

int main() {
    int should_run = 1;
    char* history = malloc(MAX_LINE);

    while(should_run){
        char* args[MAX_LINE/2 + 1] = {NULL};
        int arg_length = 0;

        printf("xsh> ");
        fflush(stdout);

        // getting the input;
        char *cmd = NULL;
        size_t cmd_length = MAX_LINE;
        getline(&cmd, &cmd_length, stdin);


        // check is history command
        // parse as argument
        if(strncmp(cmd, "!!", 2) == 0){
            parse_cmd(history, args, &arg_length);
        } else {
            strcpy(history, cmd);
            parse_cmd(cmd, args, &arg_length);
        }

        // executing
        pid_t pid = fork();
        if(pid < 0){
            printf("Unable to fork\n");
        } else if(pid == 0){
            execvp(args[0], args);
        } else {
            wait(NULL);
        }

    }
    return 0;
}

void parse_cmd(char *cmd, char *args[], int* arg_length){

    int buffer_length = 0;
    int cursor = 0;

    // remove leading empty space;
    while (cmd[cursor] == ' ') cmd +=1;

    while (1) {
        if(cmd[cursor] == ' ' || cmd[cursor] == 10){
            // copy arg
            args[*arg_length] = malloc(sizeof(char) * buffer_length);
            strncpy(args[(*arg_length)++], cmd, buffer_length);

            // if new line the cmd is end
            if(cmd[cursor] == 10) break;

            // reset buffer length
            cmd += buffer_length + 1;
            buffer_length = 0;
            cursor = 0;
        }

        cursor++;
        buffer_length++;
    }
}