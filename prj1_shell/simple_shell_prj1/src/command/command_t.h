#ifndef COMMAND_T_H
#define COMMAND_T_H

#include <stdbool.h>



typedef struct {
    char **args;           
    char *input_file;                                                                // input as file?

    char *output_file;                                                                  // output to file I guess?
    bool append_output;
} command_t;

void execute_command(command_t* command, int in_fd, int out_fd);
void print_command(command_t* cmd); 
void free_command(command_t* cmd);
#endif