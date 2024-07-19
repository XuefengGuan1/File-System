#include <unistd.h>
#include <sys/stat.h>

#include <string.h>
#include <stdlib.h>
#include "fsUtil.h"
#include "mfs.h"

void parsePath(const char *path_str, Path *path) {

    path->is_absolute = (path_str[0] == '/');
    path->token_count = 0;

    char path_copy[MAX_PATH_LENGTH];
    strncpy(path_copy, path_str, MAX_PATH_LENGTH);
    path_copy[MAX_PATH_LENGTH - 1] = '\0'; 

  
    char *token = strtok(path_copy, "/");
    
    while (token != NULL) {
        if (path->token_count < MAX_TOKENS) {
          
            strncpy(path->tokens[path->token_count], token, MAX_PATH_LENGTH);
            path->tokens[path->token_count][MAX_PATH_LENGTH - 1] = '\0'; 

            strncpy(path->last_token, token, MAX_PATH_LENGTH);
            path->last_token[MAX_PATH_LENGTH - 1] = '\0'; 

            path->token_count++;
        }

     
        token = strtok(NULL, "/");
    }
}