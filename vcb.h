/**************************************************************
 * Class::  CSC-415-02 Spring 2024
 * Name::Xuefeng Guan, Joseph Gard, Min Oo, Inderpaul Bhander
 * Student IDs::920016536, 921772888, 922837952, 922590731
 * GitHub-Name::XuefengGuan1
 * Group-Name::Team-A
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
