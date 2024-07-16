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

#include "directoryEntry.h"
#include <stdio.h>
#include "freespace.h"
#include "fsLow.h"
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>

int createRootDir(int freespaceSize, int blockSize)
{
    DirectoryEntry rootDir[DIRECTORY_ENTRY_NUMBER];

    int byteNeeded = sizeof(rootDir);
    int blockNeeded = (byteNeeded + blockSize - 1) / blockSize;
    printf("%d blocks needed\n", blockNeeded);
    for (int i = 0; i < DIRECTORY_ENTRY_NUMBER; i++)
    {
        rootDir[i].isOccupied = 0;
    }

    // Allocating memory for the blocks that are assigned to file/dir in freespace
    // first element of block_numbers will be the start location of any given file
    // or dir. block_numnbers can be used for LBA write of given file or dir.

    // Init the .
    strcpy(rootDir[0].name, ".");
    rootDir[0].isDirect = 1;
    rootDir[0].isOccupied = 1;
    time(&rootDir[0].creationTime);
    time(&rootDir[0].modificationTime);
    time(&rootDir[0].accessTime);
    rootDir[0].size = byteNeeded;

    // This only works for the root directory, child directory needs to point to its parent
    //  Init the ..
    strcpy(rootDir[1].name, "..");
    rootDir[1].isDirect = 1;
    rootDir[1].isOccupied = 1;
    time(&rootDir[1].creationTime);
    time(&rootDir[1].modificationTime);
    time(&rootDir[1].accessTime);
    rootDir[1].size = byteNeeded;

    for (int i = 0; i < blockNeeded; i++)
    {
        LBAwrite(&rootDir, 1, freespaceSize);
    }
    printf("the current block is: %d\n", freespaceSize);

    return freespaceSize;
}
