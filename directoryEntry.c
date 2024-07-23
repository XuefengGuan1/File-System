#include "directoryEntry.h"
#include <stdio.h>
#include "freespace.h"
#include "fsLow.h"
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

int blockSizeGlobal;
int blocksNeededGlobal;

int8_t createDir(int startingBlock, int blockSize, DirectoryEntry *parent)
{
    blockSizeGlobal = blockSize;
    DirectoryEntry dir[DIRECTORY_ENTRY_NUMBER];
    int byteNeeded = sizeof(dir);
    int blocksNeeded = (byteNeeded + blockSize - 1) / blockSize;
    blocksNeededGlobal = blocksNeeded;
    printf("%d blocks needed\n", blocksNeeded);

    // Initialize all entries
    for (int i = 2; i < DIRECTORY_ENTRY_NUMBER; i++)
    {
        strncpy(dir[i].name, "", MAX_FILENAME_SIZE);
        dir[i].isOccupied = 0;
        dir[i].isDirect = 0;
        dir[i].creationTime = 0;
        dir[i].modificationTime = dir[i].creationTime;
        dir[i].accessTime = dir[i].creationTime;
        dir[i].size = 0;
        dir[i].location = 0;
    }

    // Init the "." entry
    strncpy(dir[0].name, ".", MAX_FILENAME_SIZE);
    dir[0].isDirect = 1;
    dir[0].isOccupied = 1;
    time(&dir[0].creationTime);
    dir[0].modificationTime = dir[0].creationTime;
    dir[0].accessTime = dir[0].creationTime;
    dir[0].size = byteNeeded;
    dir[0].location = startingBlock;

    // Init the ".." entry
    if (parent == NULL)
    {
        // This runs if this is the root directory
        strncpy(dir[1].name, "..", MAX_FILENAME_SIZE);
        dir[1].isDirect = 1;
        dir[1].isOccupied = 1;
        dir[1].creationTime = dir[0].creationTime;
        dir[1].modificationTime = dir[0].creationTime;
        dir[1].accessTime = dir[0].creationTime;
        dir[1].size = byteNeeded;
        dir[1].location = startingBlock;
    }
    else
    {
        strncpy(dir[1].name, "..", MAX_FILENAME_SIZE);
        dir[1].isDirect = 1;
        dir[1].isOccupied = 1;
        dir[1].creationTime = parent[0].creationTime;
        time(&dir[1].modificationTime);
        dir[1].accessTime = dir[1].modificationTime;
        dir[1].size = parent[0].size;
        dir[1].location = parent[0].location;
    }

    // Allocate the blocks for the directory entries
    int *allocatedBlocks = allocateBlocks(blocksNeeded, startingBlock);
    if (allocatedBlocks == NULL)
    {
        printf("Failed to allocate blocks for directory\n");
        return -1;
    }
    printf("Directory starts at block: %d\n", allocatedBlocks[0]);

    // Write the directory entries to the allocated blocks
    int entriesPerBlock = blockSize / sizeof(DirectoryEntry);
    int entryIndex = 0;

    for (int i = 0; i < blocksNeeded; i++)
    {
        DirectoryEntry blockEntries[entriesPerBlock];
        int numEntries = (i == blocksNeeded - 1) ? (DIRECTORY_ENTRY_NUMBER - entryIndex) : entriesPerBlock;

        for (int j = 0; j < numEntries; j++)
        {
            blockEntries[j] = dir[entryIndex++];
        }

        if (LBAwrite(blockEntries, 1, allocatedBlocks[i]) != 1)
        {
            printf("Error writing directory block %d\n", i);
            free(allocatedBlocks);
            return -1;
        }
    }

    free(allocatedBlocks);
    return (int8_t)allocatedBlocks[0];
}
