#ifndef FSUTIL_H
#define FSUTIL_H
#include <stdbool.h>
#include "directoryEntry.h"
#define MAX_PATH_LENGTH 4096
#define MAX_TOKENS 256

typedef struct
{
    bool is_absolute;
    char *tokens[MAX_TOKENS];
    int token_count;
} Path;

Path *parsePath(char *path);
DirectoryEntry *getRootDirectoryEntry();
DirectoryEntry *getDirectory(DirectoryEntry *currentDirectory, char *dirGoingTo);
int makeDirectory(DirectoryEntry *currentDirectory, char *childName);
void deleteDirectory(DirectoryEntry *currentDirectory, char *childName);

#endif
