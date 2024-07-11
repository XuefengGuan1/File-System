/**************************************************************
* Class::  CSC-415-02 Spring 2024
* Name::Inderpaul Bhander
* Student IDs::
* GitHub-Name::
* Group-Name::
* Project:: Basic File System
*
* File:: freespace.c
*
* Description:: This file contains the functions for managing free
*   space using a FAT table.
*
**************************************************************/

#ifndef DIRECTORYENTRY_H
#define DIRECTORYENTRY_H
#endif
#include <time.h>


#define DIRECTORY_ENTRY_NUMBER 56

typedef struct DirectoryEntry
{
    char name[30];           // File name              31 bytes
    char isDirect;           // Check if is a directory       1 byte
    char isOccupied;         // Check if the directory is occupied    1byte
    time_t creationTime;     // Creation time             8 bytes
    time_t modificationTime; // Last modification time       8 bytes
    time_t accessTime;       // Last access time        8 bytes
    unsigned int size;       // File size in bytes          4 bytes
    unsigned int location;   // Starting block of the file on the disk   4 bytes
} DirectoryEntry;

void createRootDir(int blockSize);
// DirectoryEntry *createDir(int, DirectoryEntry *);

