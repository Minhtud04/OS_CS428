#include "parser.h"
#include "../command/command_t.h"


#define INITIAL_BUFFER_SIZE 64
#define MAX_COMMANDS 16
#define WHITESPACE_DELIM " \t\r\n\a"
#define PIPE_DELIM "|"


pipeline_t* parse_pipeline(char *line, int* size){               // char* pipeline_t -> pipeline_t is a string !
    char *command_str[MAX_COMMANDS];
    char *token;
    int count = 0;
    token = strtok(line, PIPE_DELIM);
    while (token != NULL && count < MAX_COMMANDS){
        token += strspn(token, WHITESPACE_DELIM);
        command_str[count++] = token;
        token = strtok(NULL, PIPE_DELIM);
    }
    *size = count;
    
    pipeline_t* pipelines = malloc((count + 1) * sizeof(pipeline_t));  // +1 for NULL
    if (pipelines == NULL) {
        perror("malloc failed");
        exit(EXIT_FAILURE);
    }


    for (int i = 0; i < count; i++){
        pipelines[i] = strdup(command_str[i]);
    }
    pipelines[count] = NULL;

    return pipelines;
}




command_t* parse_command(pipeline_t p) {                                        // free in parse_command arealdy?
    int size = 0;
    char** args = split_pipeline(p, &size);
    if (!args) return NULL;

    //parse into command_t
    command_t* cmd = calloc(1, sizeof(command_t));
    if (!cmd) {
        perror("malloc command_t");
        return NULL;
    }
    cmd->args = malloc((size) * sizeof(char*));
    if (!cmd->args) {
        perror("malloc args");
        free(cmd);
        return NULL;
    }
    
    int arg_count = 0;
    for (int i = 0; args[i] != NULL; i++) {
       if (strcmp(args[i], "<") == 0) {
            if (args[++i] == NULL) { 
                fprintf(stderr, "Missing input file\n");
                return NULL;
            }
            cmd->input_file = strdup(args[i]);  
        } else if (strcmp(args[i], ">>") == 0) {
            if (args[++i] == NULL) {
                fprintf(stderr, "Missing output file\n");
                return NULL;
            }
            cmd->output_file = strdup(args[i]);
            cmd->append_output = true;
        } else if (strcmp(args[i], ">") == 0) {
            if (args[++i] == NULL) {
                fprintf(stderr, "Missing output file\n");
                return NULL;
            }
            cmd->output_file = strdup(args[i]);
            cmd->append_output = false;
        } else {
            cmd->args[arg_count++] = strdup(args[i]);  
        }
    }
    cmd->args[arg_count] = NULL;
    
    free(args);
    return cmd;
}


char** split_pipeline(pipeline_t p, int* size){
    char** args = malloc(INITIAL_BUFFER_SIZE * sizeof(pipeline_t));      //malloc to store address of  - still no reallocation here...
    int pos = 0;
    char* token = strtok(p, WHITESPACE_DELIM);
    while (token != NULL){
        args[pos] = token;
        token= strtok(NULL, WHITESPACE_DELIM);
        pos++;
    }

    args[pos] = NULL;
    *size = pos + 1;
    return args;
}