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
    int *fat;
} Freespace;
Freespace *fs;

int initializeFreeSpace(uint64_t numberOfBlocks, uint64_t blockSize)
{
    numberOfBlocksGlobal = numberOfBlocks;

    fs = (Freespace *)malloc(sizeof(Freespace));
    fs->fat = (int *)malloc(sizeof(int) * numberOfBlocks);

    int freespaceBlocks = (sizeof(int) * numberOfBlocks + blockSize - 1) / blockSize;
    for (int i = 0; i < numberOfBlocks; i++)
    {
        fs->fat[i] = FREEBLOCK;
    }
    LBAwrite(fs->fat, numberOfBlocks, 1);
    return freespaceBlocks + 1;
}

int allocateBlocks(int numOfBlocksToAllocate)
{
    uint16_t head = -1;
    uint16_t prevNode = -1;
    uint16_t blockNode;

    for (int i = 0; i < numOfBlocksToAllocate; i++)
    {
        blockNode = findFreeBlock();
        if (blockNode == -1)
        {
            printf("no more available blocks");
            return -1;
        }
        printf("runned\n");
        if (head == -1)
        {
            head = blockNode;
        }
        else
        {
            fs->fat[prevNode] = blockNode;
        }
        prevNode = blockNode;
    }
    fs->fat[prevNode] = ENDBLOCK;
    return head;
}
int findFreeBlock()
{
    for (int i = 0; i < numberOfBlocksGlobal; i++)
    {
        if (fs->fat[i] == FREEBLOCK)
        {
            return i;
        }
    }
    // If no free space available
    return -1;
}
