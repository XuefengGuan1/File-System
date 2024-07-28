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
#include "vcb.h"

typedef struct Freespace
{
    int8_t *fat;
} Freespace;
Freespace *fs;

int initializeFreeSpace(uint64_t numberOfBlocks, uint64_t blockSize)
{
    fs = (Freespace *)malloc(sizeof(Freespace));
    fs->fat = (int8_t *)malloc(sizeof(int8_t) * numberOfBlocks);

    int freespaceBlocks = (sizeof(int8_t) * numberOfBlocks + blockSize - 1) / blockSize;
    for (int i = 0; i < numberOfBlocks; i++)
    {
        if(i<40){
            fs->fat[i]=2;
        }else{
        fs->fat[i] = FREEBLOCK;
        }
    }
    LBAwrite(fs->fat, numberOfBlocks, 1);

    return freespaceBlocks + 1;
}
int allocateBlocks(int numOfBlocksToAllocate, int freespaceSize)
{
    int head = -1;
    int prevNode = -1;
    int blockNode;

    for (int i = 0; i < numOfBlocksToAllocate; i++)
    {
        blockNode = findFreeBlock(freespaceSize);
        if (blockNode == -1)
        {
            printf("No more available blocks\n");
            return -1;
        }

        if (head == -1)
        {
            head = blockNode;
        }
        else
        {
            fs->fat[prevNode] = blockNode;
        }

        fs->fat[blockNode] = ENDBLOCK; // Mark as used immediately
        prevNode = blockNode;
    }

    if (LBAwrite(fs->fat, 39, 1) == -1)
    {
        printf("Error writing FAT to disk\n");
        return -1;
    }

    return head;
}

int findFreeBlock(int freespaceSize)
{
    printf("here\n");
    printf("%d\n", freespaceSize);
    for (int i = freespaceSize; i < 19531; i++)
    {

        if (fs->fat[i] == FREEBLOCK)
        {
            return i;
        }
        printf("what is the value? %d\n", fs->fat[i]);
    }
    // If no free space available
    return -1;
}

int findNextBlock(int startBlock)
{

    if (fs->fat[startBlock] == ENDBLOCK)
    {
        return ENDBLOCK;
    }
    printf("what is the NEXT value? %d\n", fs->fat[startBlock]);
    return fs->fat[startBlock];
}

void loadFAT()
{
    fs = (Freespace *)malloc(sizeof(Freespace));
    fs->fat = (int8_t *)malloc(sizeof(int8_t) * 19531);

    // Read the FAT from disk
    int fatBlocks = (sizeof(int8_t) * 19531 + 521 - 1) / 521;
    LBAread(fs->fat, fatBlocks, 1);
}
