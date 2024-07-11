#ifndef FREESPACE_H
#define FREESPACE_H
#endif

#define FREE 0xFFFFFFFF  // A value that indicates a block is free
#define END_OF_CHAIN 0xFFFFFFFE  // A value that indicates the end of a chain
#define BLOCK_SIZE 512
#include <stdbool.h>
#include "fsLow.h"

typedef struct {
    uint64_t totalBlocks;
    uint64_t freeBlocks;
    uint32_t fatTablesize;
    uint32_t *fat;  // FAT table
} Freespace;
 
void initializeFreeSpace(Freespace *fs);
int allocateBlocks(uint64_t *block_numbers, uint64_t count);
void freeBlocks(uint64_t start_block);
bool isBlockFree(uint64_t block_number);
int saveFAT(); 
int loadFAT();