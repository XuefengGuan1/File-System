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

int fs_mkdir(const char *pathname, mode_t mode)
{

    if (!pathname || strlen(pathname) == 0 || strlen(pathname) > MAX_PATH_LENGTH)
    {

        printf("invalid parameter for oathname");
        return -1;
    }

    DirectoryEntry *getDir = (DirectoryEntry*)malloc(DIRECTORY_ENTRY_NUMBER*sizeof(DirectoryEntry));
    getDir = getRootDirectoryEntry();
    if (!getDir)
    {

        printf("is getRoot wrong       !!!!!!!!!!!\n");
        return -1;
    }



    Path* parsePathname = parsePath(pathname);
    char *lastEleName = parsePathname->tokens[parsePathname->token_count]-1;
    if (!lastEleName)
    {

        printf("is lastELE wrong       !!!!!!!!!!!\n");
        return -1;
    }

    int i = 0;
    
   // printf("did this part run       !!!!!!!!!!!\n");
    
   // printf("What is the token_count? %d\n", parsePathname->token_count);
   // printf("check value of getDIr: %d\n", getDir[i].location);

    while (i < parsePathname->token_count -1)
    {

        getDir = getDirectory(getDir, parsePathname->tokens[i]);
        
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

// int fs_isFile(char *filename)
// {

//     if (filename == NULL || strlen(filename) == 0 || strlen(filename) > MAX_PATH_LENGTH)
//     {

//         return 0; // invalid file name
//     }

//     // if not a dir, then is file
//     if (fs_isDir(filename))
//     {

//         return 0;
//     }

//     struct fs_stat buff;
//     if (fs_stat(filename, &buff) == -1)
//     {

//         return 0; // file does not exist or error in stat
//     }

//     return 1;
// }

// int fs_isDir(char *pathname)
// {

//     if (pathname == NULL || strlen(pathname) == 0 || strlen(pathname) > MAX_PATH_LENGTH)
//     {

//         return 0; // invalid path
//     }

//     Path* parsedPath = parsePath(pathname);
//     if (parsedPath->token_count == 0)
//     {

//         return -1; // error retrieving root
//     }

//     DirectoryEntry *currentDir = getRootDirectoryEntry();
//     if (currentDir == NULL)
//     {

//         errno = ENOENT;
//         return -1;
//     }

//     for (int i = 0; i < parsedPath->token_count; i++)
//     {

//         DirectoryEntry *nextDir = getDirectory(currentDir, parsedPath->tokens[i]);

//         if (nextDir == NULL || nextDir->isDirect == false)
//         {
//             errno = ENOTDIR;
//             return -1;
//         }

//         currentDir = nextDir;
//     }

//     return 1;
// }

// int fs_setcwd(char *pathname) {

//     if(pathname == NULL) {

//         return -1;
//     }

//     Path *parsedPath = parsePath(pathname);
//     if(parsedPath->token_count = 0) {

//         errno = ENOENT;
//         return -1;
//     }

//     DirectoryEntry *currentDir = getRootDirectoryEntry();
//     if(currentDir == NULL) {

//         errno = ENOENT;
//         return -1;
//     }

//     for(int i = 0; i < parsedPath->token_count; i++) {

//         DirectoryEntry * nextDir = getDirectory(currentDir, parsedPath->tokens[i]);
//         if(nextDir == NULL || nextDir->isDirect == false) {

//             errno = ENOTDIR;
//             return -1;
//         }
//         currentDir = nextDir;
//     }

//     myCwd = currentDir;

//     return 0;

// }

// char *fs_getcwd(char *pathname, size_t size)
// {

//     if (pathname == NULL || size == 0)
//     {

//         return NULL;
//     }

//     if (myCwd == NULL)
//     {

//         errno = ERANGE;
//         return NULL;
//     }

//     // construct path by traversing up the dir tree
//     DirectoryEntry *current = myCwd;
//     char tempPath[MAX_PATH_LENGTH] = "";
//     char fullPath[MAX_PATH_LENGTH] = "";

//     while (current != NULL && strcmp(current->name, "/") != 0)
//     {

//         snprintf(tempPath, MAX_PATH_LENGTH, "/%s%s", current->name, fullPath);
//         strncpy(fullPath, tempPath, MAX_PATH_LENGTH);
//         current = getDirectory(current, pathname);
//     }

//     if (strlen(fullPath) >= size)
//     {

//         errno = ERANGE;
//         return NULL;
//     }

//     strncpy(pathname, fullPath, size);
//     pathname[size - 1] = '\0';

//     return pathname;
// }

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
