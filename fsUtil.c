#include <unistd.h>
#include <sys/stat.h>
#include "mfs.h"

int fs_isFile(char* path) {

    struct stat pathStat;
    if(stat(path, &pathStat) != 0) {

        return 0;
    }

    return S_ISREG(pathStat.st_mode);
}

int fs_isDir(char* path) {

    struct stat pathStat;
    if(stat(path, &pathStat) != 0) {

        return 0;
    }

    return S_ISDIR(pathStat.st_mode);
}

int fs_mkdir(const char* path, mode_t mode) {

    if(path == NULL) {

        return -1;
    }

    if(mkdir(path, mode) != 0) {

        return -1;
    }

    return 0;
}
