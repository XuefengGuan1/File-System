#ifndef FREESPACE_H
#define FREESPACE_H
#include <stdbool.h>
typedef uint64_t;
typedef uint32_t;
#define FREE 0xFFFFFFFF  // A value that indicates a block is free
#define END_OF_CHAIN 0xFFFFFFFE  // A value that indicates the end of a chain
#define BLOCK_SIZE 512
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

#endif