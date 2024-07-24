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

int8_t createDir(int startingBlock, int blockSize, DirectoryEntry *parent, int childIndex)
{
    blockSizeGlobal = blockSize;
    DirectoryEntry *dir = (DirectoryEntry*) malloc(DIRECTORY_ENTRY_NUMBER*sizeof(DirectoryEntry)); 
    int byteNeeded = 56*64;
    printf("how many bytes needed? %d\n", byteNeeded);
    int blocksNeeded = (byteNeeded + blockSize - 1) / blockSize;
    blocksNeededGlobal = blocksNeeded;
    printf("%d blocks needed\n", blocksNeeded);

    // Initialize all entries
    for (int i = 2; i < DIRECTORY_ENTRY_NUMBER; i++)
    {
        strncpy(dir[i].name, "", MAX_FILENAME_SIZE);
        dir[i].isOccupied = false;
        dir[i].isDirect = false;
        dir[i].creationTime = 0;
        dir[i].modificationTime = dir[i].creationTime;
        dir[i].accessTime = dir[i].creationTime;
        dir[i].size = 0;
        dir[i].location = 0;
    }

    // Init the "." entry
    strncpy(dir[0].name, ".", MAX_FILENAME_SIZE);
    dir[0].isDirect = true;
    dir[0].isOccupied = true;
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
        dir[1].isDirect = true;
        dir[1].isOccupied = true;
        dir[1].creationTime = dir[0].creationTime;
        dir[1].modificationTime = dir[0].creationTime;
        dir[1].accessTime = dir[0].creationTime;
        dir[1].size = byteNeeded;
        dir[1].location = startingBlock;
    }
    else
    {
        printf("dir address: %p, parent address: %p\n", (void *)dir, (void *)parent);
        printf("sizeof(DirectoryEntry): %zu\n", sizeof(DirectoryEntry));
        dir[1].creationTime = parent[0].creationTime;
        strncpy(dir[1].name, "..", MAX_FILENAME_SIZE);
        dir[1].location = parent[0].location;
        dir[1].size = parent[0].size;
        dir[1].isDirect = true;

        dir[1].isOccupied = true;
        dir[1].modificationTime = dir[1].creationTime;
        dir[1].accessTime = dir[1].creationTime;
    }

    // Allocate the blocks for the directory entries
    int head = allocateBlocks(blocksNeeded, startingBlock);

    // Write the directory entries to the allocated blocks
    int entriesPerBlock = blockSize / sizeof(DirectoryEntry);
    int entryIndex = 0;

    printf("what is head???? %d\n", head);
    int currentMemCpyValue = 0;
    LBAwrite(dir, 1, head);
    int nextValue = findNextBlock(head);
    while (nextValue != ENDBLOCK)
    {
        currentMemCpyValue += 512;
        LBAwrite(dir + currentMemCpyValue, 1, nextValue);
        nextValue = findNextBlock(nextValue);
    }

    return head;
}

void updateParent(DirectoryEntry *directory, char *childName, int childIndex, int startBlock)
{
    printf("*****************blocks is %d\n", directory[0].location);
    strncpy(directory[childIndex].name, childName, MAX_FILENAME_SIZE);
    directory[childIndex].isDirect = 1;
    directory[childIndex].isOccupied = 1;
    time(&directory[childIndex].creationTime);
    directory[childIndex].modificationTime = directory[1].creationTime;
    directory[childIndex].accessTime = directory[1].creationTime;
    directory[childIndex].size = 64;
    directory[childIndex].location = startBlock;

    // int returnValueList[7];
    LBAwrite(directory, 1, directory[0].location);
    int returnValue = findNextBlock(directory[0].location);
    int currentMemCpyValue = 0;
    while (returnValue != ENDBLOCK)
    {
        currentMemCpyValue += 512;
        LBAwrite(directory + currentMemCpyValue, 1, returnValue);
        // returnValueList[index] = returnValue;
        returnValue = findNextBlock(returnValue);
        printf("return value %d\n", returnValue);
    }
}
