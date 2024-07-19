#include <unistd.h>
#include <sys/stat.h>

#include <string.h>
#include <stdlib.h>
#include "fsUtil.h"
#include "mfs.h"

void parsePath(const char *path_str, Path *path) {
    // Initialize the path struct
    path->is_absolute = (path_str[0] == '/');
    path->token_count = 0;

    // Make a copy of the path_str to avoid modifying the original
    char path_copy[MAX_PATH_LENGTH];
    strncpy(path_copy, path_str, MAX_PATH_LENGTH);
    path_copy[MAX_PATH_LENGTH - 1] = '\0'; // Ensure null-termination

    // Tokenize the path using strtok
    char *token = strtok(path_copy, "/");
    
    while (token != NULL) {
        if (path->token_count < MAX_TOKENS) {
            // Copy the token into the tokens array
            strncpy(path->tokens[path->token_count], token, MAX_PATH_LENGTH);
            path->tokens[path->token_count][MAX_PATH_LENGTH - 1] = '\0'; // Ensure null-termination

            // Update the last_token
            strncpy(path->last_token, token, MAX_PATH_LENGTH);
            path->last_token[MAX_PATH_LENGTH - 1] = '\0'; // Ensure null-termination

            path->token_count++;
        }

        // Get the next token
        token = strtok(NULL, "/");
    }
}