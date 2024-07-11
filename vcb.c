/**************************************************************
* Class::  CSC-415-02 Spring 2024
* Name::Inderpaul Bhander
* Student IDs::
* GitHub-Name::
* Group-Name::
* Project:: Basic File System
*
* File:: freespace.c
*
* Description:: This file contains the functions for managing free
*   space using a FAT table.
*
**************************************************************/

// VCB source file
#include "vcb.h"
#include <stdlib.h>
#include <stdio.h>
#include "fsLow.h"
#include "freespace.h"
#include "directoryEntry.h"

#define VOLUME_SIG 0xFFFF

extern Freespace *fs;
struct VolumeControlBlock vcb;

int initialization(uint64_t volumeSize, uint64_t blockSize) {

    vcb.blockSize = blockSize;
  	vcb.volumeSignature = VOLUME_SIG;  // unique signature
   	vcb.rootDirectoryLocation = 2; // block 0 is vcb and block 1 is fat
   	vcb.volumeSize = volumeSize;
   	vcb.fatTableLocation = 1; // FAT is block 1, vcb is 0, root is 2

    fs->totalBlocks=volumeSize/blockSize;
    fs->fat = (uint32_t*)malloc(volumeSize*sizeof(uint32_t));
    initializeFreeSpace(fs);

    // write vcb to disk
    if(LBAwrite(&vcb, 1, 0) != 1){

        printf("Error: faile to write to disk");
        return -1;
    }

    return 0;
}
