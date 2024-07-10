#include <directoryEntry.h>

struct DirectoryEntry *rootDir;
rootDir = createDir(50, NULL);
if (rootDir == NULL)
{
    printf("Error: Failed to create root directory\n");
    return -1;
}

// Write root directory to disk
int numBlocks = (50 * sizeof(struct DirectoryEntry) + blockSize - 1) / blockSize;
if (LBAwrite(rootDir, numBlocks, vcb.rootDirectoryLocation) != numBlocks)
{
    printf("Error: Failed to write root directory to disk\n");
    free(rootDir);
    return -1;
}

DirectoryEntry *createDir(int minEntries, DirectoryEntry *parent)
{
    int byteNeeded = minEntries * sizeof(DirectoryEntry);
    int blockNeeded = (byteNeeded + vcb.blockSize - 1) / vcb.blockSize;
    int byteToAllocate = blockNeeded + vcb.blockSize;

    DirectoryEntry *newDir = malloc(byteToAllocate);
    int totalEntries = byteToAllocate / sizeof(DirectoryEntry);

    for (int i = 2; i < totalEntries; i++)
    {
        size = 0;
        location = vcb.rootDirectoryLocation;
        name[0] = "\0"; // name sub zero is null character
    }

    int newLocation = fsallocate(blockNeeded); // adjust the parameter according to Freespace parameters

    time_t curTime = time();
    DirectoryEntry[0].location = newLocation;
    DirectoryEntry[0].size = totalEntries * sizeof(DirectoryEntry);
    strcpy(newDir[0].name, ".");
    newDir[0].isDir = true;
    newDir[0].creationTime = curTime;
    newDir[0].modificationTime = curTime;
    newDir[0].accessTime = curTime;

    DirectoryEntry *prntDir == parent;
    if (prntDir == NULL)
    {
        prntDir = newDir;
    }

    memcpy(&newDir[i].name, "..");
    return newDir;
}

void exitFileSystem()
{
    printf("System exiting\n");
}

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