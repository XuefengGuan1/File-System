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
#include "vcb.h"

int initFileSystem(uint64_t numberOfBlocks, uint64_t blockSize)
{
    printf("Initializing File System with %ld blocks with a block size of %ld\n", numberOfBlocks, blockSize);
    /* TODO: Add any code you need to initialize your file system. */

    vcb.blockSize = blockSize;
    vcb.volumeSignature = 0xFFFF;  // unique signature
    vcb.rootDirectoryLocation = 2; // block 0 is vcb and block 1 is fat
    vcb.volumeSize = numberOfBlocks * blockSize;
    vcb.fatTableLocation = 1; // FAT is block 1, vcb is 0, root is 2

    // write VCB to disk
    if (LBAwrite(&vcb, 1, 0) != 1)
    {

        printf("Error: Failed to write VCB to disk\n");
    }

    return 0;
}

struct VolumeControlBlock vcb;

int initFileSystem(uint64_t numberOfBlocks, uint64_t blockSize)
{
    printf("Initializing File System with %ld blocks with a block size of %ld\n", numberOfBlocks, blockSize);
    /* TODO: Add any code you need to initialize your file system. */

    vcb.blockSize = blockSize;
    vcb.volumeSignature = 0xFFFF;  // unique signature
    vcb.rootDirectoryLocation = 2; // block 0 is vcb and block 1 is fat
    vcb.volumeSize = numberOfBlocks * blockSize;
    vcb.fatTableLocation = 1; // FAT is block 1, vcb is 0, root is 2

    // write VCB to disk
    if (LBAwrite(&vcb, 1, 0) != 1)
    {

        printf("Error: Failed to write VCB to disk\n");
    }

    return 0;
}

void exitFileSystem()
{
    printf("System exiting\n");
}
