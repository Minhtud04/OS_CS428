#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

command_t *parse_pipeline(char *line, int *num_commands);
command_t* parse_command(char* line);
char **parse_line(char *line);
#endif 