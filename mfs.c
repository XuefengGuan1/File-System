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
    //printf("here??\n");

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
    //printf("here?\n");
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

    // char *buffer = (char *)malloc(sizeof(char) * MAX_PATH_LENGTH);
    char buffer[MAX_PATH_LENGTH];
    if (strncmp(pathname, ".", 1) == 0 || strncmp(pathname, "..", 2) == 0)
    {
        mergePath(currentWorkingDirectory, pathname, buffer);
    }
    else
    {
        strncpy(buffer, pathname, MAX_PATH_LENGTH - 1);
        buffer[MAX_PATH_LENGTH - 1] = '\0';
    }

    // currentWorkingDirectory = (char *)malloc(sizeof(char) * MAX_PATH_LENGTH);
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

<<<<<<< HEAD
    //strcpy(currentWorkingDirectory, pathname);
//    free(currentWorkingDirectory);
    currentWorkingDirectory = malloc(strlen(buffer) +1);
    strcpy(currentWorkingDirectory, buffer);
=======
    // strcpy(currentWorkingDirectory, pathname);
    free(currentWorkingDirectory);
    currentWorkingDirectory = strdup(buffer);
>>>>>>> 7285746 (implemented fs_open,read and cmd_ls)
    myCwd = getDir;

    return 0;
}

char *fs_getcwd(char *pathname, size_t size)
{
    // pathname = (char *)malloc(sizeof(char) * MAX_PATH_LENGTH);
    if (pathname == NULL || size == 0)
    {
        printf("error: pathname or size invalid");
        return NULL;
    }
    if (currentWorkingDirectory == NULL)
    {

        strncpy(pathname, "/", size - 1);
        pathname[size - 1] = '\0';
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
//    free(pathname);
=======
    pathname[size - 1] = '\0';
    // free(pathname);

>>>>>>> 7285746 (implemented fs_open,read and cmd_ls)
    return pathname;
}

fdDir *fs_opendir(const char *pathname)
{
    fdDir *fd = (fdDir*)malloc(sizeof(fdDir));

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
        printf("invalid parameter for oathname\n");
        return NULL;
    }
    Path *parsedPath = parsePath(buffer);
    if (parsedPath->token_count == 0)
    {
        DirectoryEntry *getDir = getRootDirectoryEntry();
        fd->d_reclen = getDir[0].size;
        fd->directory = getDir;
        fd->dirEntryPosition = 0;
        return fd;
    }

    DirectoryEntry *getDir = getRootDirectoryEntry();
    if (getDir == NULL)
    {

        printf("error: invalid root \n");
        return NULL;
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

            printf("get directroy error\n");
            return NULL;
        }
        i++;
    }

    fd->dirEntryPosition = 0;
    fd->directory = getDir;
    fd->d_reclen = getDir[0].size;

    return fd;
}
struct fs_diriteminfo *fs_readdir(fdDir *dirp)
{
    if (dirp == NULL || dirp->directory == NULL)
    {
        return NULL;
    }

    // Check if we've reached the end of the directory entries
    if (dirp->dirEntryPosition >= DIRECTORY_ENTRY_NUMBER)
    {
        return NULL;
    }

    // Find the next valid entry
    while (dirp->dirEntryPosition < DIRECTORY_ENTRY_NUMBER)
    {
        DirectoryEntry *currentEntry = &(dirp->directory[dirp->dirEntryPosition]);

        // Move to the next entry for the next call
        dirp->dirEntryPosition++;

        // Skip empty or unused entries
        if (!currentEntry->isOccupied)
        {
            continue;
        }

        // Fill in the fs_diriteminfo structure
        struct fs_diriteminfo *di = malloc(sizeof(struct fs_diriteminfo));
        if (di == NULL)
        {
            // Memory allocation failed
            return NULL;
        }

        di->d_reclen = sizeof(struct fs_diriteminfo);
        di->fileType = currentEntry->isDirect ? 1 : 0; // 1 for directory, 0 for file
        strncpy(di->d_name, currentEntry->name, 255);
        di->d_name[255] = '\0'; // Ensure null-termination

        return di;
    }

    // No more entries
    return NULL;
}

int fs_closedir(fdDir *dirp)
{
    if (dirp == NULL)
    {
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

    return 0;
}
