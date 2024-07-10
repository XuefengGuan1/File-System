/**************************************************************
* Class::  CSC-415-02 Spring 2024
* Name::Joseph Gard,
* Student IDs::921772888
* GitHub-Name::Xuefengguan1
* Group-Name::Team-A
* Project:: Basic File System
*
* File:: fsInit.c
*
* Description:: Main driver for file system assignment.
*
* This file is where you will start and initialize your system
*
**************************************************************/


#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "fsLow.h"
#include "mfs.h"



 struct VolumeControlBlock {
    
        unsigned int blockSize;
        unsigned int volumeSignature;
        unsigned int rootDirectoryLocation;
        unsigned int volumeSize;
        unsigned int fatTableLocation;
    };
struct DirectoryEntry{
    char name[31]; // File name 31 bytes
    char isDirect; //Check if is a directory 1 byte
    time_t creationTime; // Creation time 8 bytes
    time_t modificationTime; // Last modification time 8 bytes
    time_t accessTime; // Last access time 8 bytes
    unsigned int size; // File size in bytes 4 bytes
    unsigned int location; // Starting block of the file on the disk 4 bytes
}


    
struct VolumeControlBlock vcb;
    
int initFileSystem (uint64_t numberOfBlocks, uint64_t blockSize)
    {
        printf ("Initializing File System with %ld blocks with a block size of %ld\n", numberOfBlocks, blockSize);
        /* TODO: Add any code you need to initialize your file system. */
    
        vcb.blockSize = blockSize;
        vcb.volumeSignature = 0xFFFF; // unique signature
        vcb.rootDirectoryLocation = 2; // block 0 is vcb and block 1 is fat
        vcb.volumeSize = numberOfBlocks * blockSize;
        vcb.fatTableLocation = 1; // FAT is block 1, vcb is 0, root is 2
    
        // write VCB to disk
        if(LBAwrite(&vcb,1,0) != 1) {
    
            printf("Error: Failed to write VCB to disk\n");
        }
    
        return 0;
        }

struct DriectoryEntry;

DirectoryEntry* createDir(int minEntries, DirectoryEntry* parent)
{
    int byteNeeded = minEntries * sizeof(DirectoryEntry);
    int blockNeeded = (byteNeeded + vcb.blockSize -1) / vcb.blockSize;
    int byteToAllocate = blockNeeded + vcb.blockSize;

    DirectoryEntry* newDir = malloc(byteToAllocate);
    int totalEntries = byteToAllocate / sizeof(DirectoryEntry);

    for (int i=2; i<totalEntries; i++)
    {
        size=0;
        location = vcb.rootDirectoryLocation;
        name[0] = "\0" //name sub zero is null character
    }
    
    int newLocation = fsallocate(blockNeeded); // adjust the parameter according to Freespace parameters

    time_t curTime = time();
    DirectoryEntry[0].location = newLocation;
    DirectoryEntry[0].size = totalEntries * sizeof(DirectoryEntry)
    strcpy(newDir[0].name, ".");
    newDir[0]. isDir = true;
    newDir[0]. creationTime = curTime;
    newDir[0]. modificationTime = curTime;
    newDir[0]. accessTime = curTime;

    DirectoryEntry* prntDir == parent;
    if (prntDir == NULL)
    {
        prntDir = newDir;
    }

    memcpy(&newDir[i].name, ".." );
    return newDir;
}

void exitFileSystem ()
    {
        printf ("System exiting\n");
    }
