#include <stdbool.h>

#ifndef PIPELINE_T_H
#define PIPELINE_T_H


/*
 pipeline_t is a string 
*/
typedef char* pipeline_t;     

bool execute_pipeline(pipeline_t* pipeline_ls, int size);

#endif