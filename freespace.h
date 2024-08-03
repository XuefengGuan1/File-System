/**************************************************************
 * Class::  CSC-415-02 Spring 2024
 * Name::Xuefeng Guan, Joseph Gard, Min Oo, Inderpaul Bhander
 * Student IDs::920016536, 921772888, 922837952, 922590731
 * GitHub-Name::XuefengGuan1
 * Group-Name::Team-A
 * Project:: Basic File System
 *
 * File:: freespace.h
 *
 * Description:: This file contains the functions for managing free
 *   space using a FAT table.
 *
 **************************************************************/

#ifndef FREESPACE_H
#define FREESPACE_H

#define FREEBLOCK -1 // A value that indicates a block is free
#define ENDBLOCK -2  // A value that indicates the end of a chain
#define RESERVED -3  // A value that indicates the space is reserved
#include <stdbool.h>
#include "fsLow.h"
int initializeFreeSpace(uint64_t numberOfBlocks, uint64_t blockSize);
int allocateBlocks(int numOfBlocksToAllocate, int freespaceSize);
int findFreeBlock(int freespaceSize);
int findNextBlock(int startBlock);
void loadFAT(uint64_t numberOfBlocks, uint64_t blockSize);

#endif
