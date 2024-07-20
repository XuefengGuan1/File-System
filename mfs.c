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

    DirectoryEntry* parent = fs_opendir(parentPath);
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

int fs_isFile(char *filename) {

    return 0;
}

int fs_isDir(char *pathname) {

    return 0;
}

int fs_setcwd(char *pathname) {

    if(pathname == NULL) {

        return -1;
    }

//    if(chdir(pathname) != 0) {
//
//        return -1;
//    }

    return 0;

}

char *fs_getcwd(char *pathname, size_t size) {

    if(pathname == NULL || size == 0) {
 
        return NULL;
     }

   // initialize a buffer that holds cwd path
   // append dir names as we traverse dir
   char buffer[MAX_PATH_LENGTH];
   char temp[size + 1];
   int pos = MAX_PATH_LENGTH - 1;

   buffer[pos] = '\0';
   pos--;

   DirectoryEntry *currentDir = fs_opendir(pathname);
   if(!currentDir) {

       return NULL;
   }

   // travers current dir up to root dir and build path string in reverse order
   // initialize temp buffer with current dirs name, get its length
   // check for any buffer overflow
   // move the position pointer backwards, copy the current dirs name into new buffer position
   // remove '/'
   while(currentDir.location != /*ROOT_LOCATION*/0) {

       strncpy(temp, currentDir.name, size);
       temp[size] ='\0';

       int nameLen = strlen(temp);
       // buffer overflow
       if(pos - nameLen - 1 < 0) {

           return NULL;
       }

       pos -= nameLen;
       strncpy(&buffer[pos], temp, nameLen);
       pos--;

       // prepend /
       buffer[pos] = '/';

   }

   // buffer too small
   if(MAX_PATH_LENGTH -pos -1 > size) {

       return NULL;
   }

   strncpy(pathname, &buffer[pos], size);

   return pathname;

//    if(getcwd(pathname, size) == NULL) {
// 
//        return NULL;
//     }
//
    return pathname;
}

fdDir *fs_opendir(const char *pathname) {

DirectoryEntry currentDirEntries[DIRECTORY_ENTRY_NUMBER];


    // Allocate memory for fdDir structure
    fdDir *dirp = (fdDir *)malloc(sizeof(fdDir));
    if (!dirp) {
        perror("malloc failed");
        closedir(dir);
        return NULL;
    }
    

    fd->d_reclen = sizeof(struct fs_diriteminfo);
    fd->dirEntryPosition = 0;


    fd->di = (struct fs_diriteminfo *)malloc(fd->d_reclen * DIRECTORY_ENTRY_NUMBER);
    if (fd->di == NULL) {
        free(fd);
        errno = ENOMEM; 
        return NULL;
    }

    //fill fdDir structure with directory entries
    for (int i = 0; i < DIRECTORY_ENTRY_NUMBER; i++) {
        if (currentDirEntries[i].isOccupied) {
            fd->di[i].d_reclen = sizeof(struct fs_diriteminfo);
            fd->di[i].fileType = currentDirEntries[i].isDirect ? FT_DIRECTORY : FT_REGFILE;
            strncpy(fd->di[i].d_name, currentDirEntries[i].name, sizeof(fd->di[i].d_name) - 1);
            fd->di[i].d_name[sizeof(fd->di[i].d_name) - 1] = '\0';
        }
    }

    return fd;
}