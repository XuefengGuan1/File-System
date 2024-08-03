/**************************************************************
* Class::  CSC-415-02 Spring 2024
* Name:: Xuefeng Guan, Joseph Gard, Min Oo, Inderpaul Bhander
* Student IDs::920016536, 921772888, 922837952, 922590731
* GitHub-Name::XuefengGuan1
* Group-Name::Team-A
* Project:: Basic File System
*
* File:: b_io.h
*
* Description:: Basic File System 
*
**************************************************************/

#ifndef _B_IO_H
#define _B_IO_H
#include <fcntl.h>
#include <stdint.h>
#define MAX_PATH_LENGTH 4096
#define MAX_FILENAME_SIZE 30
#endif
typedef int b_io_fd;

b_io_fd b_open (char * filename, int flags);
int b_read (b_io_fd fd, char * buffer, int count);
int b_write (b_io_fd fd, char * buffer, int count);
int b_seek (b_io_fd fd, off_t offset, int whence);
int b_close (b_io_fd fd);



