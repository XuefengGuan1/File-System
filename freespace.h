#ifndef FREESPACE_H
#define FREESPACE_H
typedef uint64_t;
typedef uint32_t;
#define FREE 0xFFFFFFFF  // A value that indicates a block is free

typedef struct {
    uint64_t total_blocks;
    uint32_t *fat;  // FAT table
} Freespace;

void initializeFreeSpace(Freespace *fs);
int allocateBlock();
void freeBlock(uint64_t block_number);
bool isBlockFree(uint64_t block_number);

#endif