#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Uso: %s num1 [num2 ...]\n", argv[0]);
        return 1;
    }

    int sum = 0;

    for (int i = 1; i < argc; i++) {
        char *arg = argv[i];

        for (int j = 0; arg[j] != '\0'; j++) {
            if (!isdigit(arg[j])) {
                fprintf(stderr, "Erro: '%s' nao eh um numero inteiro positivo valido.\n", arg);
                return 1;
            }
        }
        sum += atoi(arg);
    }

    printf("O resultado da soma eh: %d\n", sum);

    return 0;
}

