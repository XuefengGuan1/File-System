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

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "fsLow.h"
#include "freespace.h"

uint64_t numberOfBlocksGlobal;
typedef struct Freespace
{
    int8_t *fat;
} Freespace;
Freespace *fs;

int initializeFreeSpace(uint64_t numberOfBlocks, uint64_t blockSize)
{
    numberOfBlocksGlobal = numberOfBlocks;

    fs = (Freespace *)malloc(sizeof(Freespace));
    fs->fat = (int8_t *)malloc(sizeof(int8_t) * numberOfBlocks);

    int freespaceBlocks = (sizeof(int8_t) * numberOfBlocks + blockSize - 1) / blockSize;
    for (int i = 0; i < numberOfBlocks; i++)
    {
        fs->fat[i] = FREEBLOCK;
    }
    LBAwrite(fs->fat, numberOfBlocks, 1);
    return freespaceBlocks + 1;
}
int *allocateBlocks(int numOfBlocksToAllocate, int freespaceSize)
{
    int *allocatedBlocks = (int *)malloc(sizeof(int) * numOfBlocksToAllocate);
    if (allocatedBlocks == NULL)
    {
        printf("Failed to allocate memory for allocated blocks\n");
        return NULL;
    }

    int prevNode = -1;
    int blockNode;

    for (int i = 0; i < numOfBlocksToAllocate; i++)
    {
        blockNode = findFreeBlock(freespaceSize);
        if (blockNode == -1)
        {
            printf("No more available blocks\n");
            free(allocatedBlocks);
            return NULL;
        }

        printf("Allocated block: %d\n", blockNode);

        allocatedBlocks[i] = blockNode;

        if (prevNode != -1)
        {
            fs->fat[prevNode] = blockNode;
        }

        fs->fat[blockNode] = ENDBLOCK; // Mark as used immediately
        prevNode = blockNode;
    }

    printf("Allocation complete. Head block: %d\n", allocatedBlocks[0]);

    if (LBAwrite(fs->fat, numberOfBlocksGlobal, 1) == -1)
    {
        printf("Error writing FAT to disk\n");
        free(allocatedBlocks);
        return NULL;
    }

    return allocatedBlocks;
}


int findFreeBlock(int freespaceSize)
{
    for (int i = freespaceSize; i < numberOfBlocksGlobal; i++)
    {
        if (fs->fat[i] == FREEBLOCK)
        {
            printf("runned %d in findfreeblock\n", i);
            return i;
        }
    }
    // If no free space available
    return -1;
}
