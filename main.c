#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "shell.h"

int main(int argc, char **argv)
{
    char *linha = NULL;  //linha que sera digitada pelo user
    size_t tam_linha = 0;     //tamanho da linha digitada

    while (1)
    {
        char cwd[1024];
        char *home = getenv("HOME");
        if (getcwd(cwd, sizeof(cwd)) != NULL)  //pega a localização atual do diretorio
        {
          if(home != NULL && strncmp(cwd, home, strlen(home)) == 0)
        {
          printf("\n ~%s >: ", cwd + strlen(home));
        }else{
          printf("\n%s >: ", cwd);
        }
        }else{
        perror("getcwd");
        printf("\n>: ");
    }
        if(getline(&linha, &tam_linha, stdin) == -1) //funcao getlinha recebe entradas de stdin e salva o que foi escrito e o numero de caracteres nas suas respectivas variaveis
        break; //se houver erro no ou for detectado EOF (crtl+d)
       execute_cmd(linha); //redireciona o que foi digitado para os arquivos shell
    }

    free(linha);
    return 0;
}
