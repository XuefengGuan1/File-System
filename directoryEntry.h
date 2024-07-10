#ifndef DIRECTORYENTRY.H
#define DIRECTORYENTRY.H

#include <time.h>


struct DirectoryEntry
{
    char name[31];           // File name 31 bytes
    char isDirect;           // Check if is a directory 1 byte
    time_t creationTime;     // Creation time 8 bytes
    time_t modificationTime; // Last modification time 8 bytes
    time_t accessTime;       // Last access time 8 bytes
    unsigned int size;       // File size in bytes 4 bytes
    unsigned int location;   // Starting block of the file on the disk 4 bytes
};

DirectoryEntry *createDir(int, DirectoryEntry *);

#endif