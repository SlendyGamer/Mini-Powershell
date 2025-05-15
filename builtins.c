#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h> 
#include "builtins.h"

int is_builtin(char *cmd) 
{
    return (strcmp(cmd, "cd") == 0 || strcmp(cmd, "exit") == 0 || strcmp(cmd, "pwd") == 0 || strcmp(cmd, "ls") == 0); //confere se o que foi digitado eh uma destas funcoes
}

void execute_builtin(char **args) //executa funcoes acima
{
    if (strcmp(args[0], "cd") == 0) 
    {
        if (args[1] == NULL) //se for digitado apenas "cd" e sem nenhum argumento, da erro
        {
            fprintf(stderr, "cd: missing argument\n");
        } 
        else 
        {
            if (chdir(args[1]) != 0) //tenta mudar diretorio para o argumento, se retornar -1, houve erro
            {
                perror("cd");
            }
        }
    }
    else if (strcmp(args[0], "exit") == 0) 
    {
        exit(0); //finaliza powershell
    }
    else if (strcmp(args[0], "pwd") == 0) 
    {
        char cwd[1024];
        if (getcwd(cwd, sizeof(cwd)) != NULL)  //pega a localização atual do diretorio
        {
            printf("%s\n", cwd);
        } 
        else 
        {
            perror("pwd");
        }
    }
    else if (strcmp(args[0], "ls") == 0)
    {
        DIR *d;
        struct dirent *dir;
        d = opendir(".");
        int contador = 0;
        if (d)
        {
            while ((dir = readdir(d)) != NULL)
            {
                if (dir->d_name[0] == '.')
                {
                    if ((args[1] != NULL && strcmp(args[1], "-a") == 0) || (args[2] != NULL && strcmp(args[2], "-a") == 0))
                    {
                        printf("%s\t", dir->d_name);
                    }
                }
                else
                {
                    printf("%s\t", dir->d_name); 
                }
                if (contador == 5)
                {
                    printf("\n");
                    contador = 0;
                }
                else
                {
                    contador++;
                }
            }
        }
        else
        {
            perror("ls");
        }
    }
}
