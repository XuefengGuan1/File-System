#include "directoryEntry.h"
#include <stdio.h>
#include "freespace.h"
#include "fsLow.h"

DirectoryEntry rootDir;

void createRootDir(int blockSize)
{
    int byteNeeded = sizeof(rootDir) * DIRECTORY_ENTRY_NUMBER;
    int blockNeeded = (byteNeeded + blockSize - 1) / blockSize;
    DirectoryEntry entries[DIRECTORY_ENTRY_NUMBER];
    for (int i = 0; i < DIRECTORY_ENTRY_NUMBER; i++)
    {
        entries[i].isOccupied = 0;
    }
    // Allocating memory for the blocks that are assigned to file/dir in freespace
    // first element of block_numbers will be the start location of any given file
    // or dir. block_numnbers can be used for LBA write of given file or dir.
    uint64_t *block_numbers = (uint64_t *)malloc(blockNeeded * sizeof(uint64_t));
    if (block_numbers == NULL || allocateBlocks(block_numbers, blockNeeded) != 0)
    {
        free(block_numbers);
        printf("Error: Failed to allocate blocks for root directory\n");
        return;
    }
    // Init the .
    strcpy(entries[0].name, ".");
    entries[0].isDirect = 1;
    entries[0].isOccupied = 1;
    time(&entries[0].creationTime);
    time(&entries[0].modificationTime);
    time(&entries[0].accessTime);
    entries[0].size = byteNeeded;
    entries[0].location = block_numbers[0];

    // This only works for the root directory, child directory needs to point to its parent
    //  Init the ..
    strcpy(entries[1].name, "..");
    entries[1].isDirect = 1;
    entries[1].isOccupied = 1;
    time(&entries[1].creationTime);
    time(&entries[1].modificationTime);
    time(&entries[1].accessTime);
    entries[1].size = byteNeeded;
    entries[1].location = block_numbers[0];

    for (int i = 0; i < DIRECTORY_ENTRY_NUMBER; i++)
    {
        LBAwrite(&entries, 1, block_numbers[i]);
    }
}

// rootDir = createDir(50, NULL);
// if (rootDir == NULL)
// {
//     printf("Error: Failed to create root directory\n");
//     return -1;
// }

// // Write root directory to disk
// int numBlocks = (50 * sizeof(struct DirectoryEntry) + blockSize - 1) / blockSize;
// if (LBAwrite(rootDir, numBlocks, vcb.rootDirectoryLocation) != numBlocks)
// {
//     printf("Error: Failed to write root directory to disk\n");
//     free(rootDir);
//     return -1;
// }

// DirectoryEntry *createRootDir()
// {
// }

// DirectoryEntry *createDir(int minEntries, DirectoryEntry *parent)
// {
//     int byteNeeded = minEntries * sizeof(DirectoryEntry);
//     int blockNeeded = (byteNeeded + vcb.blockSize - 1) / vcb.blockSize;
//     int byteToAllocate = blockNeeded + vcb.blockSize;

//     DirectoryEntry *newDir = malloc(byteToAllocate);
//     int totalEntries = byteToAllocate / sizeof(DirectoryEntry);

//     for (int i = 2; i < totalEntries; i++)
//     {
//         size = 0;
//         location = vcb.rootDirectoryLocation;
//         name[0] = "\0"; // name sub zero is null character
//     }

//     int newLocation = fsallocate(blockNeeded); // adjust the parameter according to Freespace parameters

//     time_t curTime = time();
//     DirectoryEntry[0].location = newLocation;
//     DirectoryEntry[0].size = totalEntries * sizeof(DirectoryEntry);
//     strcpy(newDir[0].name, ".");
//     newDir[0].isDir = true;
//     newDir[0].creationTime = curTime;
//     newDir[0].modificationTime = curTime;
//     newDir[0].accessTime = curTime;

//     DirectoryEntry *prntDir == parent;
//     if (prntDir == NULL)
//     {
//         prntDir = newDir;
//     }

//     memcpy(&newDir[i].name, "..");
//     return newDir;
// }

// void exitFileSystem()
// {
//     printf("System exiting\n");
// }

/*

If the root dir isn't initialzed, replace starting line 92 to return newDir with

    // root directory
    strcpy(newDir[0].name, ".");
    newDir[0].isDir = 1;
    newDir[0].creationTime = curTime;
    newDir[0].modificationTime = curTime;
    newDir[0].accessTime = curTime;
    newDir[0].size = totalEntries * sizeof(DirectoryEntry);
    newDir[0].location = newLocation;

    // parent directory
    strcpy(newDir[1].name, "..");
    newDir[1].isDir = 1;
    newDir[1].creationTime = curTime;
    newDir[1].modificationTime = curTime;
    newDir[1].accessTime = curTime;
    newDir[1].size = totalEntries * sizeof(DirectoryEntry);
    newDir[1].location = (parent == NULL) ? newLocation : parent->location;

    return newDir

*/