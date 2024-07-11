// VCB header file
#ifndef VCB_H
#define VCB_H

#include <sys/types.h>

#ifndef uint64_t
typedef u_int64_t uint64_t;
#endif

struct VolumeControlBlock {

    unsigned int blockSize;
    unsigned int volumeSignature;
    unsigned int rootDirectoryLocation;
    unsigned int volumeSize;
    unsigned int fatTableLocation;

};

extern struct VolumeControlBlock vcb;

// vcb initialziation function
int initialization(uint64_t volumeSize, uint64_t blockSize);

#endif
