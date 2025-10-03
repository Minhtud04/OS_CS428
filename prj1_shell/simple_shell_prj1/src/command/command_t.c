#include "command_t.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <unistd.h>     
#include <sys/wait.h>  
#include <fcntl.h>




void execute_command(command_t* command, int in_fd, int out_fd){
    if (in_fd != STDIN_FILENO) {
        dup2(in_fd, STDIN_FILENO);
        close(in_fd);
    }
    if (out_fd != STDOUT_FILENO) {
        dup2(out_fd, STDOUT_FILENO);
        close(out_fd);
    }

    if (command->input_file) {
        int fd = open(command->input_file, O_RDONLY);
        if (fd < 0) { perror("open input file"); exit(EXIT_FAILURE); }
        dup2(fd, STDIN_FILENO);
        close(fd);
    }
    if (command->output_file) {
        int flags = O_WRONLY | O_CREAT | (command->append_output ? O_APPEND : O_TRUNC);
        int fd = open(command->output_file, flags, 0644);
        if (fd < 0) { perror("open output file"); exit(EXIT_FAILURE); }
        dup2(fd, STDOUT_FILENO);
        close(fd);
    }

    if (execvp(command->args[0], command->args) == -1) {
        perror("execvp failed");
        exit(EXIT_FAILURE);
    }
    exit(0);
}



void print_command( command_t* cmd) {
    printf("*******\n");
    if (!cmd) {
        printf("command: NULL\n");
        return;
    }
    printf("command: ");
    if (cmd->args && cmd->args[0]) {
        for (int i = 0; cmd->args[i] != NULL; i++) {
            printf("%s \n", cmd->args[i]);
        }
    } else {
        printf("(no args) ");
    }

    if (cmd->input_file) {
        printf("input file: < %s \n", cmd->input_file);
    }
    if (cmd->output_file) {
        printf("output file: %s %s \n", cmd->append_output ? ">>" : ">", cmd->output_file);
    }

    printf("*******\n");
}



void free_command(command_t* cmd) {
    if (!cmd) return;

    for (int i = 0; cmd->args[i] != NULL; i++) {
        free(cmd->args[i]);
    }
    free(cmd->args);

    free(cmd->input_file);
    free(cmd->output_file);

    free(cmd);
}