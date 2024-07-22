#ifndef FSUTIL_H
#define FSUTIL_H
#include <stdbool.h>
#define MAX_PATH_LENGTH 4096
#define MAX_TOKENS 256

typedef struct {
    bool is_absolute;
    char *tokens[MAX_TOKENS];
    int token_count;
} Path;

Path parsePath(const char *path);

#endif 