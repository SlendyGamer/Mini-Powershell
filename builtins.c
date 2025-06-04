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
        
        else  //se digitou "path <caminho>" salva o caminho absoluto na variável global "caminho_salvo"
        {
        char caminho_resolvido[1024];                                         //here
        const char *entrada = args[1];                                        //here

        // se o caminho não começar com '.' ou '/', assume que é relativo ao diretório atual
        if (entrada[0] != '.' && entrada[0] != '/') {                         //here
            snprintf(caminho_resolvido, sizeof(caminho_resolvido),           //here
                     "./%s", entrada);                                        //here
            entrada = caminho_resolvido;                                     //here
        }  
        
        char *res = realpath(entrada, caminho_salvo);                         //here
        if (res == NULL) {                                                   //here
            perror("realpath");                                              //here
            fprintf(stderr, "Erro: caminho '%s' inválido ou não encontrado.\n", args[1]); //here
            caminho_salvo[0] = '\0';                                         //here
        } else {                                                             //here
            printf("caminho salvo: %s\n", caminho_salvo);                    //here
        }                                                                    //here
        }
    }
}
