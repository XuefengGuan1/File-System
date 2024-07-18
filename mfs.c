#include "mfs.h"
#include "directoryEntry.h"
#include "b_io.h"
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

#define MAX_PATH_LENGTH 4096

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

    DirectoryEntry* parent = fs_opendir(parent);
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
        free(lastEleName);
        errno = ENOMEM;
        return -1;
    }

    int parentIDX = /*find unused entry function*/(parent);
    if(parentIDX == -1) {

        free(parentPath);
        free(lastEleName);
        free(newDir);
        errno = ENOSPC;
        return -1;
    }

    memcpy(&(parent[parentIDX]), newDir, sizeof(DirectoryEntry));

    strcpy(parent[parentIDX].name, lastEleName);

    // save entry to disk
    // *here*

    free(parentPath);
    free(lastEleName);
    free(newDir);

    return 0;
}


