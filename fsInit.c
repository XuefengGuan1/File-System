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
#include "directoryEntry.h"
#define VOLUME_SIG 0xFFFF
Freespace *fs;

int initFileSystem(uint64_t numberOfBlocks, uint64_t blockSize)
{
    printf("Initializing File System with %ld blocks with a block size of %ld\n", numberOfBlocks, blockSize);
    /* TODO: Add any code you need to initialize your file system. */

    // alocate memory for vcb pointer

    uint64_t volumeSize;
    char *filename = "File_System";

    printf("here runned\n");
    //start partition
    if(startPartitionSystem(filename, &volumeSize, &blockSize) != 0) {

        printf("Error");
        return -1;
    }

    printf("here runned1\n");

    fs = (Freespace*)malloc(sizeof(Freespace));
    if (!fs) {
        perror("Failed to allocate memory for FileSystem structure");
        closePartitionSystem();
        return -1;
    }
    printf("space allocation succeed\n");
    struct VolumeControlBlock *vcbPtr = malloc(sizeof(struct VolumeControlBlock));
    printf("volume control block?\n");

    if(vcbPtr == NULL) {

        printf("failed?\n");
        printf("Error: Memeory allocation for Volume Control Block Failed\n");
        return -1;
    }
    printf("succeed?\n");
    // read block 0 int vcbPtr
    if(LBAread(vcbPtr, 1, 0) != 1) {

        printf("Error: Failed to read Volume Control Block");
       	free(vcbPtr);
       	return -1;
    }
    printf("did LBAread[0] succeed?\n");

// Check vcb signature to verify volume initialzation
    	// return 0 if already initialized
    if(vcbPtr->volumeSignature == VOLUME_SIG) {
   
       	printf("Volume is already initialized");
       	free(vcbPtr);
       	return 0;
   	}
    createRootDir(blockSize);
    printf("did we create root directory?\n");
    free(vcbPtr);
    if(initialization(volumeSize, blockSize)  != 0) {

        printf("error");
        return -1;
    }

    return 0;
}

void exitFileSystem()
{
    printf("System exiting\n");
}
