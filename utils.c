#include <stdlib.h>
#include <string.h>
#include "utils.h"

char **split_line(char *line, const char *delim) 
{
    int bufsize = 64, position = 0;
    char **tokens = malloc(bufsize * sizeof(char*));
    char *token;

    token = strtok(line, delim);
    while (token != NULL) 
    {
        tokens[position++] = token;
        if (position >= bufsize) 
        {
            bufsize += 64;
            tokens = realloc(tokens, bufsize * sizeof(char*));
        }
        token = strtok(NULL, delim);
    }
    tokens[position] = NULL;
    return tokens;
}

void free_tokens(char **tokens) 
{
    free(tokens);
}
