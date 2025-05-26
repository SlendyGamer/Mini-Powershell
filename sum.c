#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int main(int argc, char *argv[]) {
    int sum = 0;

    for (int i = 1; i < argc; i++) {                                                                   // percorre todos os argumentos passados por linha de comando
        char *arg = argv[i];                                                                           // pega o argumento atua 

        for (int j = 0; arg[j] != '\0'; j++) {                                                                                            
            if (!isdigit(arg[j])) {                                                                    // verifica se todos os argumentos passados sao DIGITOS POSITIVOS OU ZERO
                fprintf(stderr, "Erro: '%s' nao eh um numero inteiro positivo valido.\n", arg);        // se nao for DIGITO POSITIVO OU ZERO = ERRO
                return 1;                                                                              // retorna UM para IMPRIMIR NO "CODIGO DA SAIDA" (No arquivo "external.c") como 1 (que simboliza ERRO)
            }
        }

        sum += atoi(arg);                                                                              // converte o argumento para um INTEIRO e soma
    }

    printf("O resultado da soma eh: %d\n", sum);                                                

    return 0;
}
