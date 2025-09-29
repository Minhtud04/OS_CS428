
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "./parse/parser.h"

int main(int argc, char **argv){
    char *line = NULL;
    bool cont = true;
    size_t buf_size = 0;
    printf("--- Parser Test Harness ---\n");
    printf("Type 'quit' to exit, or enter a string to parse.\n\n");
    

    while (cont){
        printf("min-shell:$ ");
        if (getline(&line, &buf_size, stdin) == -1){
            cont = false;
            break;
        }
        line[strcspn(line, "\n")] = 0;       // remove last character

        int size_pipeline = 0;
        pipeline_t* pipelines = parse_pipeline(line, &size_pipeline);                      
        printf("Number of commands in pipeline: %d\n", size_pipeline);
        cont = execute_pipeline(pipelines, size_pipeline);                                
        
        if (pipelines != NULL) {
            free(pipelines);
        }
    }
    free(line);
    return EXIT_SUCCESS;
}


