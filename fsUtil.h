#ifndef FSUTIL_H
#define FSUTIL_H
#include <stdbool.h>
#define MAX_PATH_LENGTH 4096
#define MAX_TOKENS 128 
typedef struct {
    bool is_absolute;
    char last_token[MAX_PATH_LENGTH];
    char tokens[MAX_TOKENS][MAX_PATH_LENGTH];
    int token_count;
} Path;


#endif