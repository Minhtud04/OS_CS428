#include <stdio.h>
#include <stdlib.h>

#include "pipeline_t.h"
#include "../builtins/built_in.h"
#include "../command/command_t.h"
#include "../parse/parser.h"



bool execute_pipeline(pipeline_t* pipeline_ls, int size){
    //do some input output I guess?
    command_t** command_ls = malloc(size * sizeof(command_t*));
    int count = 0;

    bool cont = true;
    for (int i = 0; i < size; i++){
        pipeline_t p = pipeline_ls[i];                        
        printf("pipeline here at execute level: %s\n", p);
        command_t* command = parse_command(p);               // need to free this as well -> add all of this into a list command **, then free at the end?
        print_command(command);
        if (command){
            command_ls[count] = command;
            count++;
        }

        int is_builtin = check_builtin(command);
        if (is_builtin >= 0){
            cont = execute_builtin_command(command, is_builtin);
        } else {
            cont = execute_command(command);
        }
    }

    //free stuff
    for (int j = 0; j < count; ++j){
        free_command(command_ls[j]);
    }
    free(command_ls);

    return cont;
}


