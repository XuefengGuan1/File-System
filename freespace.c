/**************************************************************
 * Class::  CSC-415-02 Spring 2024
 * Name::Inderpaul Bhander
 * Student IDs::
 * GitHub-Name::
 * Group-Name::
 * Project:: Basic File System
 *
 * File:: freespace.c
 *
 * Description:: This file contains the functions for managing free
 *   space using a FAT table.
 *
 **************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "fsLow.h"
#include "freespace.h"
#include "fsLow.h"

// check if this block is free, if not move to the next block?

int initFreeSpace(uint64_t howManyBlocks)
{
    u_int64_t *something = (uint64_t *)malloc(sizeof(uint64_t) * howManyBlocks);
    printf("what is here : %ln", something);
    return 0;
}
