/**************************************************************
* Class::  CSC-415-0# Spring 2024
* Name::
* Student IDs::
* GitHub-Name::
* Group-Name::
* Project:: Basic File System
*
* File:: b_io.c
*
* Description:: Basic File System - Key File I/O Operations
*
**************************************************************/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>			// for malloc
#include <string.h>			// for memcpy
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "freespace.h"
#include "fsLow.h"
#include "fsUtil.h"
#include "b_io.h"
#define NUM_ENTRIES 56
#define MAXFCBS 20
#define B_CHUNK_SIZE 512
#define NUM_BLOCKS 7
#define START_BLOCK 40

typedef struct b_fcb {
    char *buf;           // holds the open file buffer
    int index;       // holds the current position in the buffer
    int buflen;       // holds how many valid bytes are in the buffer
    int numblocks;
    int currentblock;
	int size;
    DirectoryEntry *fi;       // Store file-specific information
} b_fcb;
	
b_fcb fcbArray[MAXFCBS];

int startup = 0;	//Indicates that this has not been initialized

//Method to initialize our file system
void b_init ()
	{
	//init fcbArray to all free
	for (int i = 0; i < MAXFCBS; i++)
		{
		fcbArray[i].buf = NULL; //indicates a free fcbArray
		}
		
	startup = 1;
	}

//Method to get a free FCB element
b_io_fd b_getFCB ()
	{
	for (int i = 0; i < MAXFCBS; i++)
		{
		if (fcbArray[i].buf == NULL)
			{
			return i;		//Not thread safe (But do not worry about it for this assignment)
			}
		}
		return (-1);  //all in use
	}
	DirectoryEntry *findFile(Path *parsed_path) {
    
    static DirectoryEntry fi; // Temporary static DirectoryEntry
    // Fill fi with data from directory entry
    return &fi;
	}	
// Interface to open a buffered file
// Modification of interface for this assignment, flags match the Linux flags for open
// O_RDONLY, O_WRONLY, or O_RDWR
b_io_fd b_open (char * filename, int flags)
	{
    b_io_fd returnFd;
    Path *parsed_path = parsePath(filename);
    DirectoryEntry *parentDirectory = getRootDirectoryEntry();
    
    // Locate the parent directory
    for (int i = 0; i < parsed_path->token_count - 1; i++) {
        parentDirectory = getDirectory(parentDirectory, parsed_path->tokens[i]);
        if (parentDirectory == NULL) {
            return -1; // Parent directory not found
        }
    }

    // Find the file in the parent directory
    char *fileName = parsed_path->tokens[parsed_path->token_count - 1];
    DirectoryEntry *fileEntry = NULL;
    int fileIndex = -1;
    for (int i = 0; i < NUM_ENTRIES; i++) {
        if (strcmp(parentDirectory[i].name, fileName) == 0 && parentDirectory[i].isOccupied == 1) {
            fileIndex = i;
            fileEntry = &parentDirectory[i];
            break;
        }
    }

    // Handle file creation
    if (fileEntry == NULL) {
        if (!(flags & O_CREAT)) {
            return -1; // File does not exist and O_CREAT flag is not set
        }
        
        // Find a free slot in the parent directory
        int freeSlot = -1;
        for (int i = 2; i < NUM_ENTRIES; i++) {
            if (parentDirectory[i].isOccupied == 0) {
                freeSlot = i;
                break;
            }
        }

        if (freeSlot == -1) {
            return -1; // No free slot in directory
        }

        // Allocate initial block for the new file
        int location = allocateBlocks(1, freespaceBlocksGlobal);
        if (location == -1) {
            return -1; // Failed to allocate blocks
        }

        // Update the directory entry with the new file's information
        parentDirectory[freeSlot].isOccupied = 1;
        strcpy(parentDirectory[freeSlot].name, fileName);
        parentDirectory[freeSlot].location = location;
        parentDirectory[freeSlot].size = 0;

        // Write the updated directory to disk
        updateParent(parentDirectory, parentDirectory[freeSlot].name, freeSlot, parentDirectory[freeSlot].location);
     
        fileEntry = &parentDirectory[freeSlot];
        fileIndex = freeSlot;
    } else {
        // Handle file truncation if O_TRUNC flag is set
        if (flags & O_TRUNC) {
            fileEntry->size = 0;
            // Deallocate current blocks and allocate new blocks if needed
            deallocateBlocks(fileEntry->location);
            fileEntry->location = allocateBlocks(1, freespaceBlocksGlobal);
            if (fileEntry->location == -1) {
                return -1; // Failed to allocate blocks
            }
            // Write the updated directory entry
            if (LBAwrite(parentDirectory, NUM_BLOCKS, START_BLOCK) == -1) {
                return -1; // Error writing directory to disk
            }
        }
    }

    // Handle appending data if O_APPEND flag is set
    if (flags & O_APPEND) {
        // Set the file control block index to the end of the file
        fcbArray[returnFd].index = fileEntry->size;
    } else {
        // Set initial index based on the mode
        fcbArray[returnFd].index = 0;
    }
  
	if (startup == 0) b_init();  //Initialize our system
	
	returnFd = b_getFCB();       // Get our own file descriptor
    if (returnFd == -1) {
        return -1; // No free FCBs
    }
    fcbArray[returnFd].buf = (char *)malloc(B_CHUNK_SIZE);
    if (fcbArray[returnFd].buf == NULL) {
        return -1; // Memory allocation failed
    }
    fcbArray[returnFd].buflen = 0;
    fcbArray[returnFd].numblocks = 1;
    fcbArray[returnFd].currentblock = fileEntry->location;
    fcbArray[returnFd].size = fileEntry->size;

    return returnFd; // Return the file descriptor

}

// Interface to seek function	
int b_seek (b_io_fd fd, off_t offset, int whence)
	{
	if (startup == 0) b_init();  //Initialize our system

	// check that fd is between 0 and (MAXFCBS-1)
	if ((fd < 0) || (fd >= MAXFCBS))
		{
		return (-1); 					//invalid file descriptor
		}
		
	 // Implement seek functionality based on offset and whence
    switch (whence) {
        case SEEK_SET:
            fcbArray[fd].index = offset;
            break;
        case SEEK_CUR:
            fcbArray[fd].index += offset;
            break;
        case SEEK_END:
            fcbArray[fd].index = fcbArray[fd].fi->size + offset;
            break;
        default:
            return -1;
    }

    return fcbArray[fd].index; // return the new position
	}



// Interface to write function	
int b_write (b_io_fd fd, char * buf, int count)
	{
	if (startup == 0) b_init();  //Initialize our system

	// check that fd is between 0 and (MAXFCBS-1)
	if ((fd < 0) || (fd >= MAXFCBS))
		{
		return (-1); 					//invalid file descriptor
		}
	    int bytes_written = 0;

    while (count > 0) {
        int space_in_buffer = B_CHUNK_SIZE - fcbArray[fd].index;
        int bytes_to_write = (count < space_in_buffer) ? count : space_in_buffer;

        memcpy(fcbArray[fd].buf + fcbArray[fd].index, buf + bytes_written, bytes_to_write);

        fcbArray[fd].index += bytes_to_write;
        bytes_written += bytes_to_write;
        count -= bytes_to_write;

        if (fcbArray[fd].index == B_CHUNK_SIZE) {
            LBAwrite(fcbArray[fd].buf, 1, fcbArray[fd].currentblock);
            fcbArray[fd].index = 0;
            fcbArray[fd].currentblock++;
        }
    }

    return bytes_written; // return the number of bytes written
	}



// Interface to read a buf

// Filling the callers request is broken into three parts
// Part 1 is what can be filled from the current buf, which may or may not be enough
// Part 2 is after using what was left in our buf there is still 1 or more block
//        size chunks needed to fill the callers request.  This represents the number of
//        bytes in multiples of the blocksize.
// Part 3 is a value less than blocksize which is what remains to copy to the callers buf
//        after fulfilling part 1 and part 2.  This would always be filled from a refill 
//        of our buf.
//  +-------------+------------------------------------------------+--------+
//  |             |                                                |        |
//  | filled from |  filled direct in multiples of the block size  | filled |
//  | existing    |                                                | from   |
//  | buf      |                                                |refilled|
//  |             |                                                | buf |
//  |             |                                                |        |
//  | Part1       |  Part 2                                        | Part3  |
//  +-------------+------------------------------------------------+--------+
int b_read (b_io_fd fd, char * buf, int count)
	{
	int readbytes;
	int case1;
	int case2;
	int case3;
	int bytestoreturn;
	int blockstocopy;
	int bufbytesremaing;
	if (startup == 0) b_init();  //Initialize our system

	// check that fd is between 0 and (MAXFCBS-1)
	if ((fd < 0) || (fd >= MAXFCBS))
		{
		return (-1); 					//invalid file descriptor
		}
		bufbytesremaing = fcbArray[fd].buflen - fcbArray[fd].index;
	int deliverd = (fcbArray[fd].currentblock * B_CHUNK_SIZE) - bufbytesremaing;
	if((count+ deliverd)> fcbArray[fd].fi->size){
		count = fcbArray[fd].fi->size - deliverd;
	}
	//Determine what case is required.
		if(bufbytesremaing >= count){
			case1 = count;
			case2 = 0;
			case3 = 0;
		}else{
			case1= bufbytesremaing;
			case3 = count - bufbytesremaing;
			blockstocopy = case3/ B_CHUNK_SIZE;
			case2 = blockstocopy*B_CHUNK_SIZE;
			case3= case3 - case2;
		}
		//case 1 current buf fullfills data request.
		if (case1 >0){
			memcpy(buf, fcbArray[fd].buf + fcbArray[fd].index, case1);
			fcbArray[fd].index = fcbArray[fd].index +case1;
		}
		//case 2 any data that goes beyond case 1. This is counted in blocks.
		if (case2 > 0){
			readbytes = LBAread (buf+case1, blockstocopy, fcbArray[fd].currentblock + fcbArray[fd].fi->location);
			fcbArray[fd].currentblock += blockstocopy;
			case2 = readbytes * B_CHUNK_SIZE;
		}
		//Handles exhuasted buf.
		if(case3 > 0){
			readbytes = LBAread(fcbArray[fd].buf, 1, fcbArray[fd].currentblock+fcbArray[fd].fi->location);
			readbytes = readbytes * B_CHUNK_SIZE;
			fcbArray[fd].currentblock += 1;
			fcbArray[fd].index = 0;
			fcbArray[fd].buflen = readbytes;
			memcpy(buf+case1+case2, fcbArray[fd].buf+fcbArray[fd].index, case3);
			fcbArray[fd].index = fcbArray[fd].index + case3;
		}
		
		bytestoreturn= case1+ case2+case3;
		return (bytestoreturn);
	}
	
// Interface to Close the file	
int b_close (b_io_fd fd)
	{
	
    if (startup == 0) b_init(); // Initialize our system

    // check that fd is between 0 and (MAXFCBS-1)
    if ((fd < 0) || (fd >= MAXFCBS)) {
        return -1; // invalid file descriptor
    }

    if (fcbArray[fd].buf != NULL) {
        free(fcbArray[fd].buf);
        fcbArray[fd].buf = NULL;
    }

    return 0;
}
	
