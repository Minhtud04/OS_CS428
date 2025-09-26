#include "parser.h"
#include "../command/command_t.h"

#define INITIAL_BUFFER_SIZE 64
#define MAX_COMMANDS 16
#define WHITESPACE_DELIM " \t\r\n\a"
#define PIPE_DELIM "|"


command_t *parse_pipeline(char *line, int *num_commands){
    char *command_str[MAX_COMMANDS];
    char *token;
    int count = 0;
    token = strtok(line, PIPE_DELIM);
    while (token != NULL && count < MAX_COMMANDS){
        token += strspn(token, WHITESPACE_DELIM);
        command_str[count++] = token;
        token = strtok(NULL, PIPE_DELIM);
    }
    *num_commands = count;
    command_t *command_ls = malloc(count * sizeof(command_t));
    if (!command_ls) {
        perror("malloc failed");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < count; i++) {
        command_ls[i] = parse_command(command_str[i]);
    }
    
    return command_ls;
}


command_t* parse_command(char* line){
    size_t BUFFER_SIZE = INITIAL_BUFFER_SIZE;

    char **args = parse_line(line);
    command_t cmd = { .args = NULL, .input_file = NULL, .output_file = NULL, .append_output = false }; 
    cmd.args = malloc(BUFFER_SIZE * sizeof(char*));
    if (!cmd.args) { perror("malloc error"); exit(1); }
    int arg_count = 0;
    
    for (int i = 0; args[i] != NULL; i++) {
       if (strcmp(args[i], "<") == 0) {
            if (args[++i] == NULL) { fprintf(stderr, "Missing input file\n"); /* handle */ }
            cmd.input_file = strdup(args[i]);  
        } else if (strcmp(args[i], ">>") == 0) {
            if (args[++i] == NULL) { fprintf(stderr, "Missing output file\n"); /* handle */ }
            cmd.output_file = strdup(args[i]);
            cmd.append_output = true;
        } else if (strcmp(args[i], ">") == 0) {
            if (args[++i] == NULL) { fprintf(stderr, "Missing output file\n"); /* handle */ }
            cmd.output_file = strdup(args[i]);
            cmd.append_output = false;
        } else {
            if (arg_count >= BUFFER_SIZE - 1) {  // Leave room for NULL
                BUFFER_SIZE *= 2;
                char **new_args = realloc(cmd.args, BUFFER_SIZE * sizeof(char*));  // Use realloc over malloc+copy
                if (!new_args) { perror("realloc"); exit(1); }  // Or handle gracefully
                cmd.args = new_args;
            }
            cmd.args[arg_count++] = strdup(args[i]);  
        }
    }
    cmd.args[arg_count] = NULL;
    free(args);
}


char **parse_line(char *line) {                                        // free in parse_command arealdy?
    char ** tokens = malloc(INITIAL_BUFFER_SIZE * sizeof(char*));      //malloc to store address of  - still no reallocation here...
    int pos = 0;
    char* token = strtok(line, WHITESPACE_DELIM);
    while (token != NULL){
        tokens[pos] = token;
        token= strtok(NULL, WHITESPACE_DELIM);
        pos++;
    }

    tokens[pos] = NULL;
    return tokens;
}


void free_pipeline(command_t *command_ls, int num_commands) {
    if (!command_ls) return;
    for (int i = 0; i < num_commands; i++) {
        if (command_ls[i].args) {
            for (int j = 0; command_ls[i].args[j] != NULL; j++) {
                free(command_ls[i].args[j]);
            }
            free(command_ls[i].args);
        }
        free(command_ls[i].input_file);
        free(command_ls[i].output_file);
    }
    free(command_ls);
}