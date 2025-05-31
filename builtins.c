#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h> 
#include "builtins.h"
#include "utils.h"
#include <stdbool.h>

char caminho_salvo[1024] = "";

int is_builtin(char *cmd) 
{
    return (strcmp(cmd, "cd") == 0 || strcmp(cmd, "pwd") == 0 || strcmp(cmd, "path") == 0); //confere se o que foi digitado eh uma destas funcoes
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
      return;
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
    else if (strcmp(args[0], "path") == 0)
    {
        if (args[1] == NULL)  //se digitou so "path" mostra o caminho salvo
        {
            if (strlen(caminho_salvo) == 0)
            {
                printf("nenhum caminho salvo\n");
            }
            else
            {
                printf("caminho salvo: %s\n", caminho_salvo);
            }
        }
        
        else  //se digitou "path <caminho>" salva o caminho na variável global "caminho_salvo"
        {
            strncpy(caminho_salvo, args[1], sizeof(caminho_salvo) - 1);
            caminho_salvo[sizeof(caminho_salvo) - 1] = '\0';  //garante final nulo
            printf("caminho salvo: %s\n", caminho_salvo);
        }
    }
}
