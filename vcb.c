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
    // printf("initilzation function runned?\n");
    vcb.blockSize = blockSize;
  	vcb.volumeSignature = VOLUME_SIG;  // unique signature
   	vcb.rootDirectoryLocation = 2; // block 0 is vcb and block 1 is fat
   	vcb.volumeSize = volumeSize;
   	vcb.fatTableLocation = 1; // FAT is block 1, vcb is 0, root is 2

    // printf("vcb initializd?\n");
    // fs->totalBlocks=volumeSize/blockSize;
    // printf("count total block\n");
    // fs->fat = (uint32_t*)malloc(volumeSize*sizeof(uint32_t));
    // printf("malloc in vcb??\n");
    initializeFreeSpace(fs);

    // write vcb to disk
    if(LBAwrite(&vcb, 1, 0) != 1){

        printf("Error: faile to write to disk");
        return -1;
    }

    return 0;
}
