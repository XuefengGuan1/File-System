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
    for (int i = 0; i < fs->total_blocks; i++) {
        fs->fat[i] = FREE;  
    }
    fs->fat[0] = -1;  
}
int allocateBlock() {
    for (int i = 0; i < fs->total_blocks; i++) {
        if (fs->fat[i] == FREE) { 
            fs->fat[i] = -1;  
            return i;
        }
    }
    fprintf(stderr, "No free blocks available\n");
    return -1; 
}

void freeBlock(uint64_t block_number) {
    if (block_number < 0 || block_number >= fs->total_blocks) {
        fprintf(stderr, "Invalid block number\n");
        return;
    }
    fs->fat[block_number] = FREE;  
}

bool isBlockFree(uint64_t block_number) {
    if (block_number < 0 || block_number >= fs->total_blocks) {
        fprintf(stderr, "Invalid block number\n");
        return false;
    }
    return fs->fat[block_number] == FREE;
}