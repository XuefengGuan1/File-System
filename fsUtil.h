/**************************************************************
 * Class::  CSC-415-02 Spring 2024
 * Name::Xuefeng Guan, Joseph Gard, Min Oo, Inderpaul Bhander
 * Student IDs::920016536, 921772888, 922837952, 922590731
 * GitHub-Name::XuefengGuan1
 * Group-Name::Team-A
 * Project:: Basic File System
 *
 * File:: fsUtil.h
 *
 * Description:: A file that store a bunch of helper functions
 * for our file system.
 *
 **************************************************************/
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

Path *parsePath(const char *path);
DirectoryEntry *getRootDirectoryEntry();
DirectoryEntry *getDirectory(DirectoryEntry *currentDirectory, char *dirGoingTo);
int makeDirectory(DirectoryEntry *currentDirectory, char *childName);
// void deleteDirectory(DirectoryEntry *currentDirectory, char *childName);
void mergePath(const char *leftPart, const char *rightPart, char *buffer);
DirectoryEntry *parentDirectory(DirectoryEntry *currentDirectory);

#endif
