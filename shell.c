#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "shell.h"
#include "builtins.h"
#include "utils.h"

void execute_cmd(char *line)
{
    char **args = split_line(line, " \t\n");
    if (args[0] == NULL)
    {
        free_tokens(args);
        return;
    }

    if (is_buitin(args[0]))
    {
        execute_builtin(args);
    }
    else
    {
        pid_t pid = fork();
        if (pid == 0)
        {
            execvp(args[0], args);
            perror("execvp");
            exit(EXIT_FAILURE);
        }
        else
        {
            wait(NULL);
        }
    }

    free_tokens(args);
}
