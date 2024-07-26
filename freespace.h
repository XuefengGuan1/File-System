#ifndef FREESPACE_H
#define FREESPACE_H

#define FREEBLOCK -1 // A value that indicates a block is free
#define ENDBLOCK -2  // A value that indicates the end of a chain
#include <stdbool.h>
#include "fsLow.h"
int initializeFreeSpace(uint64_t numberOfBlocks, uint64_t blockSize);
int allocateBlocks(int numOfBlocksToAllocate, int freespaceSize);
int findFreeBlock(int freespaceSize);
int findNextBlock(int startBlock);

#endif
