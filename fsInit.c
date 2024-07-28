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
#include "fsUtil.h"

int initFileSystem(uint64_t numberOfBlocks, uint64_t blockSize)
{
    char readBuffer[blockSize];
    // Read the 1st block and check if existed
    LBAread(readBuffer, 1, 0);
    char localVCB[sizeof(VolumeControlBlock)];
    memcpy(localVCB, readBuffer, sizeof(VolumeControlBlock));
    VolumeControlBlock *vcbTest = (VolumeControlBlock *)localVCB;

    // If existed, doesn't do anything
    if (vcbTest->volumeSignature == VOLUME_SIG)
    {
        printf("VCB already existed\n");
        loadFAT(numberOfBlocks, blockSize);
    }
    else
    // If not exist, initialize the VCB
    {
        printf("VCB is not initialized, creating a new VCB\n");
        if (initializeVCB(numberOfBlocks * blockSize, blockSize) != 0)
        {
            printf("initalization function error?\n");
            return -1;
        }

        // init freespace later add this in the check if vcb exist function
        int freespaceSize = initializeFreeSpace(numberOfBlocks, blockSize);
        if (freespaceSize == -1)
        {
            printf("initalization function error?\n");
            return -1;
        }

        // init root directory
        int rootDirStartingBlock = createDir(freespaceSize, blockSize, NULL, -1);
    }

    // Below this are all for testings

    // Make a bunch of directories for testing
    struct fs_stat *stat;

    
    // fs_mkdir("/d1", 1);
    // fs_setcwd("/d1");
    // fs_mkdir("./d2", 1);
    // fs_mkdir("/d1/d2/d3", 1);
    // fs_stat("/d1/d2/d3", stat);

    // fs_mkdir("/d3", 1);
    //  root = getDirectory(root,"...");
    //  makeDirectory(root, "nice");
    //  root = getDirectory(root,"nice");
    //  fs_setcwd("/.../nice");
    //  fs_mkdir("./helo",3);
    //  makeDirectory(root, "....");
    //  makeDirectory(root, ".....");
    //  makeDirectory(root, "......");
    //  makeDirectory(root, ".......");
    //  makeDirectory(root, "........");
    //  makeDirectory(root, ".........");
    //  DirectoryEntry *testFindDir = getDirectory(root, "...");
    //  makeDirectory(testFindDir, "new");
    //  makeDirectory(testFindDir, "newnew");

    // printf("testing find diretory function: %d\n", testFindDir[0].location);
    // printf("runned????\n");
    // printf("else statement?\n");
    // fs_mkdir("/.../newnewnew", 3);
    // fs_setcwd("/.../");
    // fs_mkdir("./hello",3);
    // printf("here???\n");
    // int x = fs_isFile("/.../newnewnew");
    // printf("value of x %d\n", x);

    // int x = fs_isFile("/.../newnewnew");
    // int y = fs_isDir("/.../newnewnew");
    // printf("is a file? %d, is a dir? %d\n", x, y);

    //    char *testDir_file = "/.../newnewnew";
    //    char *passWrongTest = "test should fail";

    //    if(fs_isDir(testDir_file) != 1) {
    //
    //        printf("**************test failed**************");
    //    }else{
    //
    //        printf("**********TEST PASSED********");
    //    }
    //
    //    if(fs_isFile(testDir_file) != 1) {
    //
    //        printf("**************test failed**************");
    //    }else{
    //
    //        printf("**********TEST PASSED********");
    //    }
    //    if(fs_isFile(passWrongTest) == 1) {
    //
    //        printf("**************test failed**************");
    //    }else{
    //
    //        printf("**********TEST PASSED********");
    //    }
    //

    // test parsePath
    // Path* testPath = parsePath("/welcome/hello/hi");
    // printf("test path function %s\n", testPath->tokens[0]);
    return 0;
}
void exitFileSystem()
{
    printf("System exiting\n");
}
