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

int allocateBlocks(uint64_t *block_numbers, uint64_t count) {
    uint64_t allocated_count = 0;

    for (uint64_t i = 0; i < fs->totalBlocks && allocated_count < count; i++) {
        if (fs->fat[i] == FREE) {
            block_numbers[allocated_count++] = i;
            fs->freeBlocks--;
        }
    }

    for (uint64_t i = 0; i < count; i++) {
        fs->fat[block_numbers[i]] = (i == count - 1) ? END_OF_CHAIN : block_numbers[i + 1];
        fs->freeBlocks--;
    }

    return 0;  
}

void freeBlocks(uint64_t start_block) {
    uint64_t current_block = start_block;
    while (current_block != END_OF_CHAIN) {
        uint64_t next_block = fs->fat[current_block];
        fs->fat[current_block] = FREE;
        current_block = next_block;
        fs->freeBlocks++;
    }
}

bool isBlockFree(uint64_t block_number) {
    if (block_number < 0 || block_number >= fs->totalBlocks) {
        fprintf(stderr, "Invalid block number\n");
        return false;
    }
    return fs->fat[block_number] == FREE;
}