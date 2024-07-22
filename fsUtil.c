#include "fsUtil.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

Path parsePath(const char *path) {
    Path parsed_path;
    parsed_path.is_absolute = (path[0] == '/');
    parsed_path.token_count = 0;

    char path_copy[MAX_PATH_LENGTH];
    strncpy(path_copy, path, MAX_PATH_LENGTH);
    path_copy[MAX_PATH_LENGTH - 1] = '\0';

    char *token = strtok(path_copy, "/");
    while (token != NULL && parsed_path.token_count < MAX_TOKENS) {
        parsed_path.tokens[parsed_path.token_count] = strdup(token);
        parsed_path.token_count++;
        token = strtok(NULL, "/");
    }

    return parsed_path;
}

int main() {
    char path[MAX_PATH_LENGTH] = "/home/user/docs/file.txt";
    Path parsed_path = parsePath(path);

    printf("Is absolute: %d\n", parsed_path.is_absolute);
    printf("Token count: %d\n", parsed_path.token_count);
    for (int i = 0; i < parsed_path.token_count; i++) {
        printf("Token %d: %s\n", i, parsed_path.tokens[i]);
    }

    // Free allocated memory for tokens
    for (int i = 0; i < parsed_path.token_count; i++) {
        free(parsed_path.tokens[i]);
    }

    return 0;
}
