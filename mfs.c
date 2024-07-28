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
char *currentWorkingDirectory = NULL;

int fs_mkdir(const char *pathname, mode_t mode)
{
    char *buffer = (char *)malloc(sizeof(char) * MAX_PATH_LENGTH);
    if (strncmp(pathname, ".", 1) == 0 || strncmp(pathname, "..", 2) == 0)
    {
        mergePath(currentWorkingDirectory, pathname, buffer);
    }
    else
    {
        strcpy(buffer, pathname);
    }

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

    Path *parsePathname = parsePath(buffer);
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
    while (i < parsePathname->token_count - 1)
    {

        if (strcmp(parsePathname->tokens[i], ".") == 0)
        {
            i++;
            continue;
        }
        else if (strcmp(parsePathname->tokens[i], "..") == 0)
        {
            getDir = parentDirectory(getDir);
        }
        else
        {
            getDir = getDirectory(getDir, parsePathname->tokens[i]);
        }
        if (!getDir)
        {

            printf("get directroy error");
            return -1;
        }
        i++;
    }

    if (makeDirectory(getDir, lastEleName) != 0)
    {
        printf("make dir failed\n");
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
    char *buffer = (char *)malloc(sizeof(char) * MAX_PATH_LENGTH);
    if (strncmp(pathname, ".", 1) == 0 || strncmp(pathname, "..", 2) == 0)
    {
        mergePath(currentWorkingDirectory, pathname, buffer);
    }
    else
    {
        strcpy(buffer, pathname);
    }
    printf("here??\n");

    if (pathname == NULL || strlen(pathname) == 0 || strlen(pathname) > MAX_PATH_LENGTH)
    {

        return -1; // invalid path
    }

    Path *parsedPath = parsePath(buffer);
    if (parsedPath->token_count == 0)
    {

        printf("error: invalid path parsed");
        return -1;
    }

    DirectoryEntry *getDir = getRootDirectoryEntry();
    if (getDir == NULL)
    {

        printf("error: invalid root ");
        return -1;
    }
    printf("here?\n");
    int i = 0;
    while (i < parsedPath->token_count - 1)
    {

        if (strcmp(parsedPath->tokens[i], ".") == 0)
        {
            i++;
            continue;
        }
        else if (strcmp(parsedPath->tokens[i], "..") == 0)
        {
            getDir = parentDirectory(getDir);
        }
        else
        {
            getDir = getDirectory(getDir, parsedPath->tokens[i]);
        }
        if (!getDir)
        {

            printf("get directroy error");
            return -1;
        }
        i++;
    }
    if (getDir == NULL || getDir->isDirect == false)
    {
        printf("error: not a directory");
        ;
        return -1;
    }
    return 0;
}

int fs_setcwd(char *pathname)
{

    //char *buffer = (char *)malloc(sizeof(char) * MAX_PATH_LENGTH);
    char buffer[MAX_PATH_LENGTH];
    if (strncmp(pathname, ".", 1) == 0 || strncmp(pathname, "..", 2) == 0)
    {
        mergePath(currentWorkingDirectory, pathname, buffer);
    }
    else
    {
        strncpy(buffer, pathname, MAX_PATH_LENGTH -1);
        buffer[MAX_PATH_LENGTH -1] = '\0';
    }

    //currentWorkingDirectory = (char *)malloc(sizeof(char) * MAX_PATH_LENGTH);
    if (pathname == NULL)
    {

        return -1;
    }

    Path *parsedPath = parsePath(buffer);
    if (parsedPath->token_count == 0)
    {

        printf("error: invlid pare path");
        return -1;
    }

    DirectoryEntry *getDir = getRootDirectoryEntry();
    if (getDir == NULL)
    {

        printf("error: invalid root");
        return -1;
    }

    int i = 0;
    while (i < parsedPath->token_count)
    {

        if (strcmp(parsedPath->tokens[i], ".") == 0)
        {
            i++;
            continue;
        }
        else if (strcmp(parsedPath->tokens[i], "..") == 0)
        {
            getDir = parentDirectory(getDir);
        }
        else
        {
            getDir = getDirectory(getDir, parsedPath->tokens[i]);
        }
        if (!getDir)
        {

            printf("get directroy error");
            return -1;
        }
        i++;
    }

    //strcpy(currentWorkingDirectory, pathname);
    free(currentWorkingDirectory);
    currentWorkingDirectory = strdup(buffer);
    myCwd = getDir;

    return 0;
}

char *fs_getcwd(char *pathname, size_t size)
{
    //pathname = (char *)malloc(sizeof(char) * MAX_PATH_LENGTH);
    if (pathname == NULL || size == 0)
    {
        printf("error: pathname or size invalid");
        return NULL;
    }
    if(currentWorkingDirectory == NULL) {

        strncpy(pathname, "/", size-1);
        pathname[size-1] = '\0';
    }

    // construct path by traversing up the dir tree
    if (currentWorkingDirectory == NULL)
    {
        return "/";
    }
    strncpy(pathname, currentWorkingDirectory, size);
<<<<<<< HEAD
    pathname[size - 1] = '\0';
    //free(pathname);
    
=======
    free(pathname);

>>>>>>> 6a8b0c4 (last min changes)
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

struct fs_diriteminfo *fs_readdir(fdDir *dirp) {
    if (dirp == NULL || dirp->directory == NULL) {
        return NULL;
    }

    while (dirp->dirEntryPosition < DIRECTORY_ENTRY_NUMBER) {
        DirectoryEntry *entry = &dirp->directory[dirp->dirEntryPosition];

        if (entry->isOccupied) {
            struct fs_diriteminfo *dirInfo = (struct fs_diriteminfo *)malloc(sizeof(struct fs_diriteminfo));
            if (dirInfo == NULL) {
                return NULL;
            }

            strcpy(dirInfo->d_name, entry->name);
            dirInfo->d_reclen = dirp->d_reclen;
            dirInfo->fileType = entry->isDirect;

            dirp->dirEntryPosition++;
            return dirInfo;
        }

        dirp->dirEntryPosition++;
    }

    return NULL;
}


int fs_closedir(fdDir *dirp) {
    if (dirp == NULL) {
        return -1;
    }

    free(dirp);
    return 0;
}


int fs_stat(const char *path, struct fs_stat *buf)
{
    char *buffer = (char *)malloc(sizeof(char) * MAX_PATH_LENGTH);
    if (strncmp(path, ".", 1) == 0 || strncmp(path, "..", 2) == 0)
    {
        mergePath(currentWorkingDirectory, path, buffer);
    }
    else
    {
        strcpy(buffer, path);
    }
    Path *parsedPath = parsePath(buffer);
    if (parsedPath->token_count == 0)
    {

        printf("error: invlid pare path");
        return -1;
    }
    DirectoryEntry *getDir = getRootDirectoryEntry();
    if (getDir == NULL)
    {

        printf("error: invalid root");
        return -1;
    }

    int i = 0;
    while (i < parsedPath->token_count)
    {

        if (strcmp(parsedPath->tokens[i], ".") == 0)
        {
            i++;
            continue;
        }
        else if (strcmp(parsedPath->tokens[i], "..") == 0)
        {
            getDir = parentDirectory(getDir);
        }
        else
        {
            getDir = getDirectory(getDir, parsedPath->tokens[i]);
        }
        if (!getDir)
        {

            printf("get directroy error");
            return -1;
        }
        i++;
    }
    buf->st_accesstime = getDir[0].accessTime;
    buf->st_createtime = getDir[0].creationTime;
    buf->st_modtime = getDir[0].modificationTime;
    buf->st_blocks = DIRECTORY_ENTRY_NUMBER;
    buf->st_size = getDir[0].size;
}