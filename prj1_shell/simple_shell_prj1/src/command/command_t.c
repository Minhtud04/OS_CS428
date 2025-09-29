#include "command_t.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>



bool execute_command(command_t* command){
    return true;
}

void print_command( command_t* cmd) {
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
    printf("\n");
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