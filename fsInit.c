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
#include "freespace.h"
#define VOLUME_SIG 0xFFFF
Freespace *fs;

int initFileSystem(uint64_t numberOfBlocks, uint64_t blockSize)
{
    printf("Initializing File System with %ld blocks with a block size of %ld\n", numberOfBlocks, blockSize);
    /* TODO: Add any code you need to initialize your file system. */

    VolumeControlBlock *vcbPtr = (VolumeControlBlock *)malloc(512 * sizeof(VolumeControlBlock));
    // VolumeControlBlock *ourPtr = (VolumeControlBlock*) malloc(512*sizeof(VolumeControlBlock));

    // ourPtr->volumeSignature = VOLUME_SIG
    LBAread(vcbPtr, 1, 0);

    printf("content is %d\n", vcbPtr->volumeSignature);
    if (vcbPtr->volumeSignature == VOLUME_SIG)
    {
        free(vcbPtr);
        return 0;
    }
    else
    {
        initialization(numberOfBlocks * blockSize, blockSize);
    }

    //init freespace
    initFreeSpace(numberOfBlocks);


    return 0;
}

void exitFileSystem()
{
    printf("System exiting\n");
}
