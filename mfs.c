#include "mfs.h"
#include "directoryEntry.h"
#include "b_io.h"
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

#define MAX_PATH_LENGTH 4096

// store the cwd
static char cwd[MAX_PATH_LENGTH] = "/";

fs_mkdir(const char *pathname, mode_t mode) {

    if(!pathname || strlen(pathname) == 0 || strlen(pathname) > MAX_PATH_LENGTH) {

        errno = EINVAL;
        return -1;
    }

    char *parentPath = strdup(pathname);
    if(!parentPath) {

        errno = ENOMEM;
        return -1;
    }

    DirectoryEntry * parent = fs_opendir(parentPath);
    if(!parent) {

        free(parentPath);
        errno = ENOENT;
        return -1;
    }

    const char *lastEleName = /*find last element function*/(pathname);
    if(!lastEleName) {

        errno = ENOMEM;
        free(parentPath);
        return -1;
    }

    DirectoryEntry *newDir = createDir(DEFAULT_ENTRIES, parent);
    if(!newDir) {

        free(parentPath);
        errno = ENOMEM;
        return -1;
    }

    int parentIDX = /*find unused entry function*/(parent);
    if(parentIDX == -1) {

        free(parentPath);
        free(newDir);
        errno = ENOSPC;
        return -1;
    }

    memcpy(&(parent[parentIDX]), newDir, sizeof(DirectoryEntry));

    strcpy(parent[parentIDX].name, lastEleName);

    // save entry to disk
    // *here*

    free(parentPath);
    free(newDir);

    return 0;
}

int fs_isFile(char *filename) {

    if(filename == NULL || strlen(filename) == 0 || strlen(filename) > MAX_PATH_LENGTH) {

        return 0; //invalid file name
    }

    struct fs_stat buff;
    if(fs_stat(filename, &buff) == -1) {

        return 0; // file does not exist or error in stat
    }

    // need filetype in fs_stat
    // fileType = entry.isDir ? FT_DIRECTORY : FT_REGFILE
//    if((buff.fileType = FT_REGFILE)) {
//
//        return 1; // regular file
//    }

    return 0;// not a regular file
}

int fs_isDir(char *pathname) {

    if(pathname == NULL || strlen(pathname) == 0 || strlen(pathname) > MAX_PATH_LENGTH) {

        return 0; // invalid path
    }

    fdDir *dir = fs_opendir(pathname);
    if(dir == NULL) {

        return 0; //not a directory
    }

    fs_closedir(dir);
    return 1;
}

int fs_setcwd(char *pathname) {

    if(pathname == NULL) {

        return -1;
    }

    if(!fs_isDir(pathname)) {

        errno = ENOTDIR;
        return -1;
    }

    strncpy(cwd, pathname, MAX_PATH_LENGTH);
    cwd[MAX_PATH_LENGTH - 1] = '\0';

    return 0;

}

char *fs_getcwd(char *pathname, size_t size) {

    if(pathname == NULL || size == 0) {
 
        return NULL;
     }

    if(strlen(cwd) >= size) {

        errno = ERANGE;
        return NULL;
    }

    strncpy(pathname, cwd, size);
    pathname[size-1] = '\0';

    return pathname;
}

fdDir *fs_opendir(const char *pathname) {
    // Open the directory using opendir
    DIR *dir = opendir(pathname);
    if (!dir) {
        perror("opendir failed");
        return NULL;
    }

    // Allocate memory for fdDir structure
    fdDir *dirp = (fdDir *)malloc(sizeof(fdDir));
    if (!dirp) {
        perror("malloc failed");
        closedir(dir);
        return NULL;
    }

    // Initialize fdDir structure
    dirp->d_reclen = 0;
    dirp->dirEntryPosition = 0;
    dirp->di = NULL;

    dirp->dirStream = dir;

    return dirp;
}
