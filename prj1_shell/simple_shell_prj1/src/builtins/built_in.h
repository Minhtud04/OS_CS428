#ifndef BUILT_IN_H
#define BUILT_IN_H

#include <stdbool.h>
#include "../command/command_t.h"
#include <stdbool.h>
#include "../pipeline/pipeline_t.h"


typedef struct {
    char *name;
    bool (*function)(char **);
} builtin_t;

extern const builtin_t builtins[];

int check_builtin(command_t* c);

bool execute_cd(char** args);

bool execute_exit(char** args);

bool execute_builtin_command(command_t* c, int pos);

#endif