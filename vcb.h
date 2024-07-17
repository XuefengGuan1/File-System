/**************************************************************
 * Class::  CSC-415-02 Spring 2024
 * Name::Inderpaul Bhander
 * Student IDs::
 * GitHub-Name::
 * Group-Name::
 * Project:: Basic File System
 *
 * File:: vcb.h
 *
 * Description:: Header file for volume control block
 *
 **************************************************************/

// VCB header file
#ifndef VCB_H
#define VCB_H

#include "directoryEntry.h"
#include "vcb.h"

#define VOLUME_SIG 1234

typedef struct VolumeControlBlock
{
    unsigned int volumeSignature;
    unsigned int blockSize;
    unsigned int rootDirectoryLocation;
    unsigned int volumeSize;
    unsigned int fatTableLocation;

} VolumeControlBlock;

// vcb initialziation function
int initializeVCB(uint64_t volumeSize, uint64_t blockSize);

#endif
