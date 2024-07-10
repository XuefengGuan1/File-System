// VCB header file
#ifndef VCB_H
#define VCB_H

struct VolumeControlBlock {

    unsigned int blockSize;
    unsigned int volumeSignature;
    unsigned int rootDirectoryLocation;
    unsigned int volumeSize;
    unsigned int fatTableLocation;

};

extern struct VolumeControlBlock vcb;

#endif
