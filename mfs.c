#include "mfs.h"
#include "directoryEntry.h"
#include "b_io.h"
#include "freespace.h"
#include "fsUtil.h"
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

#define MAX_PATH_LENGTH 4096
DirectoryEntry *myCwd = NULL;
char *currentWorkingDirectory;

int fs_mkdir(const char *pathname, mode_t mode)
{

    if (!pathname || strlen(pathname) == 0 || strlen(pathname) > MAX_PATH_LENGTH)
    {
        printf("invalid parameter for oathname");
        return -1;
    }

    DirectoryEntry *getDir = getRootDirectoryEntry();
    if (!getDir)
    {

        printf("error: invalid root\n");
        return -1;
    }

    Path *parsePathname = parsePath(pathname);
    if (!parsePathname)
    {

        printf("parsed pathname error");
        return -1;
    }
    char *lastEleName = parsePathname->tokens[parsePathname->token_count - 1];
    if (!lastEleName)
    {

        printf("error: invalid last element\n");
        return -1;
    }

    int i = 0;

    // printf("did this part run       !!!!!!!!!!!\n");

    // printf("What is the token_count? %d\n", parsePathname->token_count);
    // printf("check value of getDIr: %d\n", getDir[i].location);

    while (i < parsePathname->token_count - 1)
    {

        getDir = getDirectory(getDir, parsePathname->tokens[i]);
        if (!getDir)
        {

            printf("get directroy error");
            return -1;
        }

        // printf("check value of getDIr: %d\n", getDir[i].location);

        i++;
    }

    if (makeDirectory(getDir, lastEleName) != 0)
    {

        printf("make dir failed");
        return -1;
    }

    return 0;
}

int fs_isFile(char *filename)
{

    if (filename == NULL || strlen(filename) == 0 || strlen(filename) > MAX_PATH_LENGTH)
    {

        return -1; // invalid file name
    }

    // if not a dir, then is file
    if (fs_isDir(filename) == 0)
    {

        return -1;
    }

    return 0;
}

int fs_isDir(char *pathname)
{

    if (pathname == NULL || strlen(pathname) == 0 || strlen(pathname) > MAX_PATH_LENGTH)
    {

        return -1; // invalid path
    }

    Path *parsedPath = parsePath(pathname);
    if (parsedPath->token_count == 0)
    {

        printf("error: invalid path parsed");
        return -1;
    }

    DirectoryEntry *currentDir = getRootDirectoryEntry();
    if (currentDir == NULL)
    {

        printf("error: invalid root ");
        return -1;
    }

    for (int i = 0; i < parsedPath->token_count; i++)
    {

        DirectoryEntry *nextDir = getDirectory(currentDir, parsedPath->tokens[i]);

        if (nextDir == NULL || nextDir->isDirect == false)
        {
            printf("error: not a directory");
            ;
            return -1;
        }

        currentDir = nextDir;
    }

    return 0;
}

int fs_setcwd(char *pathname)
{
    currentWorkingDirectory = (char *)malloc(sizeof(char) * MAX_PATH_LENGTH);
    if (pathname == NULL)
    {

        return -1;
    }

    Path *parsedPath = parsePath(pathname);
    if (parsedPath->token_count == 0)
    {

        printf("error: invlid pare path");
        return -1;
    }

    DirectoryEntry *currentDir = getRootDirectoryEntry();
    if (currentDir == NULL)
    {

        printf("error: invalid root");
        return -1;
    }

    for (int i = 0; i < parsedPath->token_count; i++)
    {
        DirectoryEntry *nextDir = getDirectory(currentDir, parsedPath->tokens[i]);
        if (nextDir == NULL || nextDir->isDirect == false)
        {

            printf("error: not a directory");
            return -1;
        }
        currentDir = nextDir;
    }

    strcpy(currentWorkingDirectory, pathname);
    myCwd = currentDir;
    printf("what is my current working directory's address? %d\n", myCwd[0].location);

    return 0;
}

char *fs_getcwd(char *pathname, size_t size)
{
    pathname = (char*) malloc(sizeof(char) * MAX_PATH_LENGTH);
    if (pathname == NULL || size == 0)
    {

        printf("error: pathname or size invalid");
        return NULL;
    }
    printf("runne here\n");
    // if (myCwd == NULL)
    // {

    //     printf("error: myCwd null");
    //     return NULL;
    // }
    printf("happened here?>\n");
    // construct path by traversing up the dir tree
    strncpy(pathname, currentWorkingDirectory, size);
    printf("what is the path %s\n", pathname);
    free(pathname);
    return pathname;
}

fdDir *fs_opendir(const char *pathname)
{

    int count;

    Path *pathcopy = parsePath(pathname);

    DirectoryEntry *dir = getRootDirectoryEntry();

    int i = 0;
    while (pathcopy->tokens[i] == NULL)
    {
        DirectoryEntry *dir = getDirectory(dir, pathcopy->tokens[i]); // current loaded directory
    }

    fdDir *fd = (fdDir *)malloc(sizeof(fdDir));
    if (fd == NULL)
    {
        return NULL; // Memory allocation failed
    }

    fd->d_reclen = sizeof(fdDir);
    fd->dirEntryPosition = 0;
    fd->directory = dir;

    return fd;
}

// struct fs_diriteminfo *fs_readdir(fdDir *fd)
// {
//     if (fd == NULL || fd->directory == NULL || fd->dirEntryPosition >= fd->d_reclen)
//     {
//         return NULL;
//     }

//     DirectoryEntry* entry = &fd->directory[fd->dirEntryPosition];
//     static struct fs_diriteminfo diriteminfo;

//     diriteminfo.d_reclen = sizeof(struct fs_diriteminfo);
//     diriteminfo.fileType = entry->isDirect ? FT_DIRECTORY : FT_REGFILE;
//     strncpy(diriteminfo.d_name, entry->name, 255);
//     fd->dirEntryPosition++;

//     return &diriteminfo;
// }

// int fs_rmdir(const char *pathname){
//     Path parsed = parsePath(pathname);
//     if (cwd == parsed.tokens[parsed.tokens[parsed.token_count-1]]);
//     //if getDir true;
//     //call deleteDE

//     if (parsed.is_absolute){

//     }

// }

// int fs_closedir(fdDir *fd)
// {
//     if (fd == NULL)
//     {
//         return -1;
//     }

//     free(fd);
//     return 0;
// }
