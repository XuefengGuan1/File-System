/**************************************************************
 * Class::  CSC-415-02 Spring 2024
 * Name::Inderpaul Bhander
 * Student IDs::
 * GitHub-Name::
 * Group-Name::
 * Project:: Basic File System
 *
 * File:: vcb.c
 *
 * Description:: Initialize the volume control block
 * call free space, and then write the vcb to disk
 *
 **************************************************************/

// VCB source file
#include "freespace.h"
#include "vcb.h"
#include <stdlib.h>
#include <stdio.h>
#include "fsLow.h"
#include <sys/types.h>


int initializeVCB(uint64_t volumeSize, uint64_t blockSize)
{
    VolumeControlBlock vcb;

    // printf("initilzation function runned?\n");
    vcb.blockSize = blockSize;
    vcb.volumeSignature = VOLUME_SIG; // unique signature
    vcb.rootDirectoryLocation = 2;    // block 0 is vcb and block 1 is fat
    vcb.volumeSize = volumeSize;
    vcb.fatTableLocation = 1; // FAT is block 1, vcb is 0, root is 2

    // write vcb to disk
    if (LBAwrite(&vcb, 1, 0) != 1)
    {
        printf("Error: faile to write to disk");
        return -1;
    }

    return 0;
}
