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

DirectoryEntry rootDir;

int createRootDir(int blockSize)  
{
    int byteNeeded = sizeof(rootDir) * DIRECTORY_ENTRY_NUMBER;
    int blockNeeded = (byteNeeded + blockSize - 1) / blockSize;
    DirectoryEntry entries[DIRECTORY_ENTRY_NUMBER];
    for (int i = 0; i < DIRECTORY_ENTRY_NUMBER; i++)
    {
        entries[i].isOccupied = 0;
    }

    // printf("did init entries in DE work?\n");
    // Allocating memory for the blocks that are assigned to file/dir in freespace
    // first element of block_numbers will be the start location of any given file
    // or dir. block_numnbers can be used for LBA write of given file or dir.

    // int *x = (int*)malloc(5*sizeof(int));
    // printf("debug malloc worked?\n");

    uint64_t *block_numbers = (u_int64_t *)malloc(blockNeeded * sizeof(uint64_t));
    // printf("did the malloc work?\n");

    if (block_numbers == NULL || allocateBlocks(block_numbers, blockNeeded) != 0)
    {
        free(block_numbers);
        printf("Error: Failed to allocate blocks for root directory\n");
        return EXIT_FAILURE;
    }
    // printf("did the allocation run?\n");
    // Init the .
    strcpy(entries[0].name, ".");
    entries[0].isDirect = 1;
    entries[0].isOccupied = 1;
    time(&entries[0].creationTime);
    time(&entries[0].modificationTime);
    time(&entries[0].accessTime);
    entries[0].size = byteNeeded;
    entries[0].location = block_numbers[0];

    // This only works for the root directory, child directory needs to point to its parent
    //  Init the ..
    strncpy(entries[1].name, "..", 29);
    entries[1].name[29] = '\n';
    entries[1].isDirect = 1;
    entries[1].isOccupied = 1;
    time(&entries[1].creationTime);
    time(&entries[1].modificationTime);
    time(&entries[1].accessTime);
    entries[1].size = byteNeeded;
    entries[1].location = block_numbers[0];

    for (int i = 3; i < 10; i++)
    {
        LBAwrite(&entries, 1, i);
        printf("the current block is: %d\n", i);
    }
    return block_numbers[0];
}
