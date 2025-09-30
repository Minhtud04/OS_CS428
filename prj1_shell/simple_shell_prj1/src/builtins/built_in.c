#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "built_in.h"
#include <string.h>

const builtin_t builtins[] = {
    {"cd", execute_cd},    // No &—function decay to pointer in C
    {"exit", execute_exit},
};


int check_builtin(command_t* c){

    int pos = -1;
    if (c->args == NULL || c->args[0] == NULL) {
        return -1;
    }
    char *first_arg = c->args[0];
    int size_ = sizeof(builtins) / sizeof(builtin_t);
    for (int i = 0; i < size_; i++) {
        if (strcmp(first_arg, builtins[i].name) == 0) {
            pos = i; 
            break;
        }
    }
    return pos;
}


bool execute_cd(char** args){
    char *target_dir = args[1];

    if (target_dir == NULL) {
        target_dir = getenv("HOME");
        if (target_dir == NULL) {
            fprintf(stderr, "min-shell: cd: HOME environment variable not set\n");
            return true;
        }
    }
    if (chdir(target_dir) != 0) {
        perror("min-shell: cd");
    }
    return true;
}


bool execute_exit(char** args){
    return false;
}

bool execute_builtin_command(command_t* c, int pos){
    return builtins[pos].function(c->args);
}
