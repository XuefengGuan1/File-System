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

Path *parsePath(const char *path)
{
    // Allocate memory for the Path structure
    Path *parsed_path = (Path *)malloc(sizeof(Path));
    if (parsed_path == NULL)
    {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    // Initialize the Path structure
    parsed_path->is_absolute = (path[0] == '/');
    parsed_path->token_count = 0;

    // Make a copy of the path to avoid modifying the original string
    char *path_copy = strdup(path);
    if (path_copy == NULL)
    {
        fprintf(stderr, "Memory allocation failed\n");
        free(parsed_path);
        exit(EXIT_FAILURE);
    }

    // Tokenize the path and store tokens
    char *token = strtok(path_copy, " \t/");
    while (token != NULL)
    {
        if (parsed_path->token_count >= MAX_TOKENS)
        {
            fprintf(stderr, "Exceeded maximum number of tokens\n");
            break;
        }
        parsed_path->tokens[parsed_path->token_count] = strdup(token);
        if (parsed_path->tokens[parsed_path->token_count] == NULL)
        {
            fprintf(stderr, "Memory allocation failed\n");
            // Free previously allocated tokens
            for (int i = 0; i < parsed_path->token_count; i++)
            {
                free(parsed_path->tokens[i]);
            }
            free(parsed_path);
            free(path_copy);
            exit(EXIT_FAILURE);
        }
        parsed_path->token_count++;
        token = strtok(NULL, " \t/");
    }

    free(path_copy); // Free the temporary copy

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

    printf("what is the parent's directory in the getDIrectory function? %d\n", currentDirectory[0].location);

    int dirExists = 0;
    int entryPosition = -1;

    for (int i = 0; i < NUM_ENTRIES; i++)
    {
        int result = strcmp(currentDirectory[i].name, dirGoingTo);
        if (result == 0 && currentDirectory[i].isOccupied == 1 && currentDirectory[i].isDirect == 1)
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

void mergePath(const char *leftPart, const char *rightPart, char *buffer)
{
    strcpy(buffer, leftPart);
    strcat(buffer, rightPart);
    printf("what is the result? %s\n", buffer);
}

DirectoryEntry *parentDirectory(DirectoryEntry *currentDirectory)
{
    char *buffer = malloc(56 * 64);
    int parentDirectoryLocation = currentDirectory[1].location;
    while (1)
    {
        LBAread(buffer, 1, parentDirectoryLocation);
        if (findNextBlock(parentDirectoryLocation) != ENDBLOCK)
        {
            break;
        }
    }

    DirectoryEntry *parentDirectory = (DirectoryEntry *)buffer;

    return parentDirectory;
}

// void deleteDirectory(DirectoryEntry *currentDirectory, char *childName)
// {
// }
