#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include "shell.h"

int main(int argc, char *argv)
{
    char *line = NULL;
    size_t len = 0;

    while (1)
    {
        printf(">: ");
        if(getline(&line, &len, stdin) == -1)
        {
            break;
        }
        //execute_cmd(line);
    }

    free(line);
    return 0;
}
