#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shell.h"

int main(int argc, char **argv)
{
    char *linha = NULL;  //linha que sera digitada pelo user
    size_t tam_linha = 0;     //tamanho da linha digitada

    while (1)
    {
        printf("\n>: ");  //texto simples por estetica, para indicar que o usuario pode digitar
        if(getline(&linha, &tam_linha, stdin) == -1) //funcao getlinha recebe entradas de stdin e salva o que foi escrito e o numero de caracteres nas suas respectivas variaveis
        {
            break; //se houver algum erro ou for detectado EOF (crtl+d)
        }
        execute_cmd(linha); //redireciona o que foi digitado para os arquivos shell
    }

    free(linha);
    return 0;
}
