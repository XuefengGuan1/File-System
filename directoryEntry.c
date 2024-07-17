/**************************************************************
 * Class::  CSC-415-02 Spring 2024
 * Name::Inderpaul Bhander
 * Student IDs::
 * GitHub-Name::
 * Group-Name::
 * Project:: Basic File System
 *
 * File:: directoryEntry.c
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

int blockSizeGlobal;
int blocksNeededGlobal;

int16_t createDir(int startingBlock, int blockSize, DirectoryEntry *parent)
{
    blockSizeGlobal = blockSize;
    DirectoryEntry dir[DIRECTORY_ENTRY_NUMBER];
    int byteNeeded = sizeof(dir);
    int blocksNeeded = (byteNeeded + blockSize - 1) / blockSize;
    blocksNeededGlobal = blocksNeeded;
    printf("%d blocks needed\n", blocksNeeded);

    // Initialize all entries
    for (int i = 2; i < DIRECTORY_ENTRY_NUMBER; i++)
    {
        dir[i].isOccupied = 0;
    }

    // Init the "." entry
    strncpy(dir[0].name, ".", MAX_FILENAME_SIZE);
    dir[0].isDirect = 1;
    dir[0].isOccupied = 1;
    time(&dir[0].creationTime);
    dir[0].modificationTime = dir[0].creationTime;
    dir[0].accessTime = dir[0].creationTime;
    dir[0].size = byteNeeded;
    dir[0].location = startingBlock;

    // Init the ".." entry
    if (parent == NULL)
    {
        // This runs if this is the root directory
        strncpy(dir[1].name, "..", MAX_FILENAME_SIZE);
        dir[1].isDirect = 1;
        dir[1].isOccupied = 1;
        dir[1].creationTime = dir[0].creationTime;
        dir[1].modificationTime = dir[0].creationTime;
        dir[1].accessTime = dir[0].creationTime;
        dir[1].size = byteNeeded;
        dir[0].location = startingBlock;
    }
    else
    {
        strncpy(dir[1].name, "..", MAX_FILENAME_SIZE);
        dir[1].isDirect = 1;
        dir[1].isOccupied = 1;
        dir[1].creationTime = parent[0].creationTime;
        time(&dir[1].modificationTime);
        dir[1].accessTime = dir[1].modificationTime;
        dir[1].size = parent[0].size;
        dir[0].location = parent[0].location;
    }

    int16_t startingBlockFromFreeSpace = allocateBlocks(blocksNeeded, startingBlock);
    if (startingBlockFromFreeSpace == -1)
    {
        printf("Failed to allocate blocks for root directory\n");
        return -1;
    }
    printf("Root directory starts at block: %d\n", startingBlockFromFreeSpace);

    int head = startingBlockFromFreeSpace;
    int entriesPerBlock = blockSize / sizeof(DirectoryEntry);

    for (int i = 0; i < blocksNeeded; i++)
    {
        int entriesToWrite = (i == blocksNeeded - 1) ? (DIRECTORY_ENTRY_NUMBER % entriesPerBlock) : entriesPerBlock;
        if (entriesToWrite == 0)
            entriesToWrite = entriesPerBlock;

        if (LBAwrite(dir + i * entriesPerBlock, 1, head) != 1)
        {
            printf("Error writing root directory block %d\n", i);
            return -1;
        }
        head++;
    }

    return startingBlockFromFreeSpace;
}
