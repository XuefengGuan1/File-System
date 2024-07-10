/**************************************************************
* Class::  CSC-415-0# Spring 2024
* Name::
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
#include "freespace.h"

extern Freespace *fs;

// FREE macro defined in freespace.h. Value of -1 for reserved blocks
void initializeFreeSpace(Freespace *fs) {
    for (int i = 0; i < fs->totalBlocks; i++) {
        fs->fat[i] = FREE;  
    }
    fs->fat[0] = -1;  
    fs->fat[1] = -1;
    fs->fat[2] = -1;
    fs->freeBlocks = fs->totalBlocks-3;
}
int allocateBlock() {
    for (int i = 0; i < fs->totalBlocks; i++) {
        if (fs->fat[i] == FREE) { 
            fs->fat[i] = -1;  
            fs->freeBlocks--;
            return i;
        }
    }
    fprintf(stderr, "No free blocks available\n");
    return -1; 
}

void freeBlock(uint64_t blockNumber) {
    if (blockNumber < 0 || blockNumber >= fs->totalBlocks) {
        fprintf(stderr, "Invalid block number\n");
        return;
    }
    fs->fat[blockNumber] = FREE;  
    fs->freeBlocks++;
}

bool isBlockFree(uint64_t blockNumber) {
    if (blockNumber < 0 || blockNumber >= fs->totalBlocks) {
        fprintf(stderr, "Invalid block number\n");
        return false;
    }
    return fs->fat[blockNumber] == FREE;
}