#ifndef FREESPACE_H
#define FREESPACE_H
#endif

#define FREEBLOCK -1                // A value that indicates a block is free
#define END_OF_CHAIN 0xFFFFFFFE // A value that indicates the end of a chain
#include <stdbool.h>
#include "fsLow.h"

int initializeFreeSpace(uint64_t numberOfBlocks, uint64_t blockSize);
int allocateBlocks(int);
void freeBlocks(uint64_t start_block);
bool isBlockFree(uint64_t block_number);
int saveFAT();
int loadFAT();