#include "fsUtil.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "vcb.h"
#include "freespace.h"
// Assuming 512 is the block size; adjust if different
#define BLOCK_SIZE 512
#define NUM_BLOCKS 7
#define START_BLOCK 40
#define NUM_ENTRIES 56

Path parsePath(const char *path)
{
    Path parsed_path;
    parsed_path.is_absolute = (path[0] == '/');
    parsed_path.token_count = 0;

    char path_copy[MAX_PATH_LENGTH];
    strncpy(path_copy, path, MAX_PATH_LENGTH);
    path_copy[MAX_PATH_LENGTH - 1] = '\0';

    char *token = strtok(path_copy, "/");
    while (token != NULL && parsed_path.token_count < MAX_TOKENS)
    {
        parsed_path.tokens[parsed_path.token_count] = strdup(token);
        parsed_path.token_count++;
        token = strtok(NULL, "/");
    }

    return parsed_path;
}

DirectoryEntry *getRootDirectoryEntry()
{
    char buffer[NUM_BLOCKS * BLOCK_SIZE];
    if (LBAread(buffer, NUM_BLOCKS, START_BLOCK) == -1)
    {
        printf("Error reading blocks\n");
        return NULL;
    }

    DirectoryEntry *rootDir = (DirectoryEntry *)buffer;
    for (int i = 0; i < DIRECTORY_ENTRY_NUMBER; i++)
    {
        printf("Entry %d: isDirect = %d, name = %s\n", i, rootDir[i].isDirect, rootDir[i].name);
    }

    printf("try Root %d\n", rootDir[0].isDirect);
    return rootDir;
}

DirectoryEntry *getDirectory(DirectoryEntry *currentDirectory, char *dirGoingTo)
{
    if (currentDirectory == NULL || dirGoingTo == NULL)
    {
        printf("Invalid input parameters\n");
        return NULL;
    }

    int dirExists = 0;
    int entryPosition = -1;

    for (int i = 0; i < NUM_ENTRIES; i++)
    {
        int result = strcmp(currentDirectory[i].name, dirGoingTo);
        if (result == 0 && currentDirectory[i].isOccupied == 1)
        {
            entryPosition = currentDirectory[i].location;
            dirExists = 1;
            break;
        }
        printf("Comparing '%s' with '%s', dir value is %d, result is %d\n",
               currentDirectory[i].name, dirGoingTo, dirExists, result);
    }

    printf("Directory search complete\n");
    if (dirExists == 0)
    {
        printf("Directory not found\n");
        return NULL;
    }
    printf("Directory found at position %d\n", entryPosition);

    DirectoryEntry *dir = malloc(NUM_BLOCKS * BLOCK_SIZE);
    if (dir == NULL)
    {
        printf("Memory allocation failed\n");
        return NULL;
    }

    if (LBAread(dir, NUM_BLOCKS, entryPosition) == -1)
    {
        printf("Error reading blocks\n");
        free(dir);
        return NULL;
    }

    printf("dir location is %d\n", dir[0].location);
    return dir;
}

int makeDirectory(DirectoryEntry *currentDirectory, char *childName)
{
    printf("what about in create directory?%d\n", currentDirectory[0].location);
    int freeSlot = -1;
    for (int i = 2; i < NUM_ENTRIES; i++) // Start from 2 to skip "." and ".."
    {
        if (currentDirectory[i].isOccupied == 0)
        {
            freeSlot = i;
            break;
        }
    }

    if (freeSlot == -1)
    {
        printf("No free slots in directory\n");
        return -1;
    }

    int freeBlock = findFreeBlock(41);
    if (freeBlock == -1)
    {
        printf("No free blocks available\n");
        return -1;
    }
    updateParent(currentDirectory, childName, freeSlot, freeBlock);
    createDir(freeBlock, 512, currentDirectory, freeSlot);

    return 0;
}

void deleteDirectory(DirectoryEntry *currentDirectory, char *childName)
{
}

// int main() {
//     char path[MAX_PATH_LENGTH] = "/home/user/docs/file.txt";
//     Path parsed_path = parsePath(path);

//     printf("Is absolute: %d\n", parsed_path.is_absolute);
//     printf("Token count: %d\n", parsed_path.token_count);
//     for (int i = 0; i < parsed_path.token_count; i++) {
//         printf("Token %d: %s\n", i, parsed_path.tokens[i]);
//     }

//     // Free allocated memory for tokens
//     for (int i = 0; i < parsed_path.token_count; i++) {
//         free(parsed_path.tokens[i]);
//     }

//     return 0;
// }
