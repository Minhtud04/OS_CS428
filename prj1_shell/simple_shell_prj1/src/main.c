
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "./parse/parser.h"

int main(int argc, char **argv){
    char *line = NULL;
    bool stop = false;
    size_t buf_size = 0;
    printf("--- Parser Test Harness ---\n");
    printf("Type 'quit' to exit, or enter a string to parse.\n\n");


    while (!stop){
        printf("min-shell: $");
        if (getline(&line, &buf_size, stdin) == -1){
            stop = true;
            break;
        }
        line[strcspn(line, "\n")] = 0;       //remove last character
        if (strcmp(line, "quit") == 0) {
            stop = true;
            break;
        }
        if (strlen(line) == 0) {
            continue;
        }
        
        
    }

    return EXIT_SUCCESS;
}
