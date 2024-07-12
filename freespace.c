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
#include "fsLow.h"

extern Freespace *fs;

// FREE macro defined in freespace.h. Value of -1 for reserved blocks
// takes into account if FAT size is larger then one block.
void initializeFreeSpace(Freespace *fs) {
    // printf("did the freespace function run?\n");
    fs->fatTablesize = (fs->totalBlocks * sizeof(uint32_t) + BLOCK_SIZE - 1) / BLOCK_SIZE;
    for (int i = 0; i < fs->totalBlocks; i++) {
        fs->fat[i] = FREE;  
    }
    fs->fat[0] = -1;
    fs->fat[1] = -1;
    fs->freeBlocks = fs->totalBlocks-1;  
    for (uint64_t i = 1; i <= fs->fatTablesize; i++) {
        fs->fat[i] = -1;
        fs->freeBlocks--;
    }
    saveFAT();
    
}

// iterate through fat table to find free blocks and provide startblock
// to calling function. link blocks in fat table.
int allocateBlocks(uint64_t *block_numbers, uint64_t count) {
    // printf("did the allocation function run?\n");
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

int saveFAT() {
     uint64_t fatSize = (fs->totalBlocks * sizeof(uint32_t) + BLOCK_SIZE - 1) / BLOCK_SIZE;  // Number of blocks the FAT table occupies
    printf("Saving FAT to disk, size in blocks: %lu\n", fatSize);  // Debug print
    if (LBAwrite(fs->fat, fatSize, 1) != fatSize) {  // Assuming FAT starts at block 1
        perror("Failed to write FAT to disk::");
        return -1;
    }
    return 0;
}


int loadFAT() {
    fs->fatTablesize = (fs->totalBlocks * sizeof(uint32_t) + BLOCK_SIZE - 1) / BLOCK_SIZE;  
    if (LBAread(fs->fat, fs->fatTablesize, 1) != fs->fatTablesize) {  
        perror("Failed to read FAT from disk::");
        return -1;
    }
    return 0;
}