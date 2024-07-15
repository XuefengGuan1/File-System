#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>

// PATH_MAX was not defined in my limits.h file
// fallback value 
#ifndef PATH_MAX
#define PATH_MAX 4096
#endif

#include "fsCWD.h"

char* fs_getCWD(void) {


    // allocate memory for path name
    // set size using limits.h PATH_MAX 4096 bytes
    char* buffer = malloc(PATH_MAX);

    if(buffer == NULL) {

        free(buffer);
        return NULL;
    }

    if(getcwd(buffer, PATH_MAX) == NULL) {

        free(buffer);
        return NULL;
    }

    return buffer;
}

int fs_setCWD(const char* path) {

    if(path == NULL) {

        return -1;
    }

    if(chdir(path) != 0) {

        return -1;
    }

    return 0;
}
