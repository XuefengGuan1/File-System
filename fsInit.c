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
#include "fsUtil.h"

int initFileSystem(uint64_t numberOfBlocks, uint64_t blockSize)
{
    char readBuffer[blockSize];
    // Read the 1st block and check if existed
    LBAread(readBuffer, 1, 0);
    char localVCB[sizeof(VolumeControlBlock)];
    memcpy(localVCB, readBuffer, sizeof(VolumeControlBlock));
    VolumeControlBlock *vcb = (VolumeControlBlock *)localVCB;

    // If existed, doesn't do anything
    if (vcb->volumeSignature == VOLUME_SIG)
    {
        printf("VCB already existed\n");
    }
    else
    // If not exist, initialize the VCB
    {
        printf("VCB is not initialized, creating a new VCB\n");
        if (initializeVCB(numberOfBlocks * blockSize, blockSize) != 0)
        {
            printf("initalization function error?\n");
            return -1;
        }
    }
    // init freespace later add this in the check if vcb exist function
    int freespaceSize = initializeFreeSpace(numberOfBlocks, blockSize);
    if (freespaceSize == -1)
    {
        printf("initalization function error?\n");
        return -1;
    }

    // init root directory
    int rootDirStartingBlock = createDir(freespaceSize, blockSize, NULL);

    getRootDirectoryEntry();
    return 0;
}
void exitFileSystem()
{
    printf("System exiting\n");
}
