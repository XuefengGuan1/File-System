#ifndef FSCWD_H
#define FSCWD_H

// getter for current working directory
// returns pointer to a string that contains the current working dir
// after every get call, caller frees allocated memory
char* fs_getCWD(void);

// setter for cwd
// takes a pointer of a string that holds the new path to set
// return a 0 for complete, -1 for fail
int fs_setCWD(const char* path);

#endif
