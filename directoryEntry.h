// /**************************************************************
// * Class::  CSC-415-02 Spring 2024
// * Name::Inderpaul Bhander
// * Student IDs::
// * GitHub-Name::
// * Group-Name::
// * Project:: Basic File System
// *
// * File:: freespace.c
// *
// * Description:: This file contains the functions for managing free
// *   space using a FAT table.
// *
// **************************************************************/

#ifndef DIRECTORYENTRY_H
#define DIRECTORYENTRY_H
#include <time.h>
#include "fsLow.h"
#include <stdint.h>
#include <stdbool.h>

#define DIRECTORY_ENTRY_NUMBER 56
#define MAX_FILENAME_SIZE 30

typedef struct DirectoryEntry
{
    // 64 bytes in total
    char name[MAX_FILENAME_SIZE]; // File name              30 bytes
    bool isDirect;                // Check if is a directory       1 byte
    bool isOccupied;              // Check if the directory is occupied    1 byte
    time_t creationTime;          // Creation time             8 bytes
    time_t modificationTime;      // Last modification time       8 bytes
    time_t accessTime;            // Last access time        8 bytes
    unsigned int size;            // File size in bytes          4 bytes
    unsigned int location;        // Starting block of the file on the disk   4 bytes
} DirectoryEntry;

int8_t createDir(int startingBlock, int blockSize, DirectoryEntry *parent, int childIndex);

// Don't use the updateParent function.
void updateParent(DirectoryEntry *directory, char *childName, int childIndex, int startBlock, int isDir);

#endif
