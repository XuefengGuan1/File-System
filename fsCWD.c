#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>

#include "mfs.h"

char* fs_getcwd(char* path, size_t size) {


    if(path == NULL || size == 0) {

        return NULL;
    }

    if(getcwd(path, size) == NULL) {

        return NULL;
    }

    return path;
}

int fs_setcwd(char* path) {

    if(path == NULL) {

        return -1;
    }

    if(chdir(path) != 0) {

        return -1;
    }

    return 0;
}
