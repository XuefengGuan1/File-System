#include "fsLow.h"
#include "mfs.h"

fdDir* fs_opendir(const char *pathname) {
    // Define and initialize local variables
    fdDir *fd = NULL;
    DirectoryEntry parentDirEntries[DIRECTORY_ENTRY_NUMBER];
    DirectoryEntry currentDirEntries[DIRECTORY_ENTRY_NUMBER];
    char parentPath[256];
    char lastElement[MAX_FILENAME_SIZE];
    int parentBlock = -1;
    int currentBlock = -1;
    int found = 0;

    // 1. Parse the path into parent directory path and last element (directory name)
    const char *lastSlash = strrchr(pathname, '/');
    if (lastSlash) {
        // Extract parent path
        snprintf(parentPath, sizeof(parentPath), "%.*s", (int)(lastSlash - pathname), pathname);
        // Extract last element
        snprintf(lastElement, sizeof(lastElement), "%s", lastSlash + 1);
    } else {
        // If there's no slash, treat the whole path as the last element and parent is current directory
        snprintf(parentPath, sizeof(parentPath), ".");
        snprintf(lastElement, sizeof(lastElement), "%s", pathname);
    }

    // 2. Find the block number of the parent directory
    parentBlock = getBlockForPath(parentPath); // Simulate finding block for parent directory

    // 3. Read the parent directory entries
    readBlockFromDisk(parentBlock, parentDirEntries, sizeof(parentDirEntries)); // Simulate reading from disk

    // 4. Locate the last element in the parent directory
    for (int i = 0; i < DIRECTORY_ENTRY_NUMBER; i++) {
        if (strcmp(parentDirEntries[i].name, lastElement) == 0) {
            if (parentDirEntries[i].isDirect) {
                currentBlock = parentDirEntries[i].location;
                found = 1;
            }
            break;
        }
    }

    if (!found) {
        errno = ENOTDIR; // Last element is not a directory
        return NULL;
    }

    // 5. Load the current directory entries from the block
    readBlockFromDisk(currentBlock, currentDirEntries, sizeof(currentDirEntries)); // Simulate reading from disk

    // 6. Allocate memory for fdDir
    fd = (fdDir *)malloc(sizeof(fdDir));
    if (fd == NULL) {
        errno = ENOMEM; // Out of memory
        return NULL;
    }

    // 7. Initialize fdDir structure
    fd->d_reclen = sizeof(struct fs_diriteminfo);
    fd->dirEntryPosition = 0;
    fd->dirStream = NULL; // Not used, but setting to NULL
    fd->di = (struct fs_diriteminfo *)malloc(fd->d_reclen * DIRECTORY_ENTRY_NUMBER);
    if (fd->di == NULL) {
        free(fd);
        errno = ENOMEM; // Out of memory
        return NULL;
    }

    // 8. Populate fdDir structure with directory entries
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
