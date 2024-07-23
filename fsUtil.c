#include "fsUtil.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "vcb.h"
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
    int dirExists = 0;
    int entryPosition;
    for (int i = 0; i < NUM_ENTRIES; i++)
    {
        if (currentDirectory[i].name == dirGoingTo)
        {
            entryPosition = currentDirectory[i].location;
            dirExists = 1;
            break;
        }
    }
    if (dirExists == 0)
    {
        return NULL;
    }

    char buffer[NUM_BLOCKS * BLOCK_SIZE];
    if (LBAread(buffer, NUM_BLOCKS, entryPosition) == -1)
    {
        printf("Error reading blocks\n");
        return NULL;
    }

    DirectoryEntry *dir = (DirectoryEntry *)buffer;
    return dir;
}

int makeDirectory(DirectoryEntry *currentDirectory, char *newDir)
{
}
void deleteDirectory(DirectoryEntry *currentDirectory, char *newDir)
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
