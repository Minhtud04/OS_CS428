#include <stdio.h>
#include <stdlib.h>

#include "pipeline_t.h"
#include "../builtins/built_in.h"
#include "../command/command_t.h"
#include "../parse/parser.h"

#include <unistd.h>     
#include <sys/wait.h>  
#include <fcntl.h>


bool execute_pipeline(pipeline_t* pipeline_ls, int size){
    //do some input output I guess?
    command_t** command_ls = malloc(size * sizeof(command_t*));
    int count = 0;
    pid_t pids[size];
    int in_fd = STDIN_FILENO;
    bool cont = true;

    for (int i = 0; i < size; i++){
        pipeline_t p = pipeline_ls[i];                        
        command_t* command = parse_command(p);               // need to free this as well -> add all of this into a list command **, then free at the end?
        // print_command(command);
        if (command){
            command_ls[count] = command;
            count++;
        }
                                                                            //builtin
        int is_builtin = check_builtin(command);
        if (is_builtin >= 0) {
            cont = execute_builtin_command(command, is_builtin);
            continue;
        }

        int pipe_fds[2];
        int out_fd = STDOUT_FILENO;

        if (i < size - 1) {
            if (pipe(pipe_fds) == -1) {                                 //connecting the pipe woth pipe() system call
                perror("pipe");
                break; 
            }
            out_fd = pipe_fds[1];
        }

        
        pid_t pid = fork();
        if (pid == 0){
            execute_command(command, in_fd, out_fd);
        }
        pids[i] = pid;
        if (in_fd != STDIN_FILENO) {close(in_fd);}
        if (out_fd != STDOUT_FILENO) {close(out_fd);}                   //close read/write IO file
        if (i < size - 1) { in_fd = pipe_fds[0];}                       // continue the pipeline
    }

     for (int i = 0; i < count; i++) {
        if (pids[i] > 0) {
            int status;
            waitpid(pids[i], &status, 0);
            if (!WIFEXITED(status) || WEXITSTATUS(status) != 0) {cont = false;}
        }
    }

    //free stuff
    for (int j = 0; j < count; ++j){
        free_command(command_ls[j]);
    }
    free(command_ls);

    return cont;
}


