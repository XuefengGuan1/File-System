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

int createRootDir(int startingBlock, int blockSize)
{
    blockSizeGlobal = blockSize;
    DirectoryEntry rootDir[DIRECTORY_ENTRY_NUMBER];
    int byteNeeded = sizeof(rootDir);
    int blocksNeeded = (byteNeeded + blockSize - 1) / blockSize;
    blocksNeededGlobal = blocksNeeded;
    printf("%d blocks needed\n", blocksNeeded);

    // Initialize all entries
    for (int i = 0; i < DIRECTORY_ENTRY_NUMBER; i++)
    {
        rootDir[i].isOccupied = 0;
    }

    // Init the "." entry
    strncpy(rootDir[0].name, ".", MAX_FILENAME_SIZE);
    rootDir[0].isDirect = 1;
    rootDir[0].isOccupied = 1;
    time(&rootDir[0].creationTime);
    rootDir[0].modificationTime = rootDir[0].creationTime;
    rootDir[0].accessTime = rootDir[0].creationTime;
    rootDir[0].size = byteNeeded;
    rootDir[0].location = startingBlock;

    // Init the ".." entry
    strncpy(rootDir[1].name, "..", MAX_FILENAME_SIZE);
    rootDir[1].isDirect = 1;
    rootDir[1].isOccupied = 1;
    rootDir[1].creationTime = rootDir[0].creationTime;
    rootDir[1].modificationTime = rootDir[0].creationTime;
    rootDir[1].accessTime = rootDir[0].creationTime;
    rootDir[1].size = byteNeeded;
    rootDir[0].location = startingBlock;

    int startingBlockFromFreeSpace = allocateBlocks(blocksNeeded, startingBlock);
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

        if (LBAwrite(rootDir + i * entriesPerBlock, 1, head) != 1)
        {
            printf("Error writing root directory block %d\n", i);
            return -1;
        }
        head++;
    }

    return startingBlockFromFreeSpace;
}

uint16_t createDirectory(DirectoryEntry *parent)
{
    DirectoryEntry newDir[DIRECTORY_ENTRY_NUMBER];

    for (int i = 0; i < DIRECTORY_ENTRY_NUMBER; i++)
    {
        newDir[i].isOccupied = 0;
    }

    strncpy(newDir[0].name, ".", MAX_FILENAME_SIZE);
    newDir[0].isDirect = 1;
    newDir[0].isOccupied = 1;
    time(&newDir[0].creationTime);
    newDir[0].modificationTime = newDir[0].creationTime;
    newDir[0].accessTime = newDir[0].creationTime;
    newDir[0].size = sizeof(newDir);

    strncpy(newDir[1].name, "..", MAX_FILENAME_SIZE);
    newDir[1].isDirect = 1;
    newDir[1].isOccupied = 1;
    time(&newDir[1].creationTime);
    newDir[1].modificationTime = newDir[1].creationTime;
    newDir[1].accessTime = newDir[1].creationTime;
    newDir[1].size = parent[0].size;

    
}