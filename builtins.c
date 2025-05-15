#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h> 
#include "builtins.h"
#include "utils.h"
#include <stdbool.h>

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
        FileInfo info;

        bool flag_a = false;
        bool flag_l = false;

        for (int i=1; args[i] != NULL; i++)
        {
            if (strcmp(args[i], "-a") == 0)
                flag_a = true;
            if (strcmp(args[i], "-l") == 0)
                flag_l = true;
            if (strcmp(args[i], "-al") == 0 || strcmp(args[i], "-la") == 0)
            {
                flag_a = true;
                flag_l = true;
            }
        }
        if (d)
        {
            while ((dir = readdir(d)) != NULL)
            {
                if (dir->d_name[0] == '.' && flag_a) //invisivel e -a
                {
                    if (flag_l)
                    {
                        info = getFileInfo(dir->d_name);//invisivel e -a -l
                        printf("%s %2ld %s %s %5ld %s %s\n",
                                info.permissions,
                                info.links,
                                info.owner,
                                info.group,
                                info.size,
                                info.mod_time,
                                info.name);
                    }
                    else //invisivel e -a
                    {
                        printf("%s\t", dir->d_name);
                        contador++;
                    }
                }
                else if (flag_l)//visivel e -l (tanto faz se for -a ou nao)
                {
                    info = getFileInfo(dir->d_name);
                    printf("%s %2ld %s %s %5ld %s %s\n",
                            info.permissions,
                            info.links,
                            info.owner,
                            info.group,
                            info.size,
                            info.mod_time,
                            info.name);
                }
                else //visivel
                {
                    printf("%s\t", dir->d_name);
                    contador++;
                }                    
                if (contador == 5)
                {
                    printf("\n");
                    contador = 0;
                }
            }
        }
        else
        {
            perror("ls");
        }
    }
}
