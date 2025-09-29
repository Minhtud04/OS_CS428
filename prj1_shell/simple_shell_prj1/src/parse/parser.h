#define _GNU_SOURCE
#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../pipeline/pipeline_t.h"
#include "../command/command_t.h"


pipeline_t *parse_pipeline(char *line, int* size);
command_t* parse_command(pipeline_t p);


char** split_pipeline(pipeline_t p, int* size);
#endif 