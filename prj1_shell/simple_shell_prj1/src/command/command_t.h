#ifndef COMMAND_T_H
#define COMMAND_T_H

#include <stdbool.h>



typedef struct {
    char **args;           
    char *input_file;                                                                // input as file?

    char *output_file;                                                                  // output to file I guess?
    bool append_output;
} command_t;

typedef struct {
    char *name;
    bool (*function)(command_t*);
} builtin_t;

const builtin_t builtins[] = {
    {"cd", &execute_cd},
    {"exit", &execute_exit}
};

void execute_pipeline(command_t* command);

void execute_command(command_t* command);

bool check_builtin(command_t* command);

bool execute_builtin_command(command_t* command);

bool execute_cd(command_t* command);

bool execute_exit(command_t* command);

#endif