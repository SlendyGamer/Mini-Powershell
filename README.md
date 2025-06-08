## Projeto Mini-Powershell
Um mini shell feito na linguagem C para o sistema operacional linux

# Principais funcionalidades
Os comandos: 
  - exit: Finaliza o shell.
  - cd <caminho>: Muda o diretório de trabalho.
  - path <caminho> [<caminho> <caminho> ...] : Define caminho(s) para busca de executáveis.
  - pwd : Exibir o caminho do diretório atual.
  - cat <arquivo> lê o conteúdo do arquivo no argumento e o escreve na saída padrão.
  - ls: lista o conteúdo do diretório atual. Seu ls deve suportar os parâmetros -l e -a conforme o funcionamento do ls original.

# Pre-requisitos
  - Git
  - Make
  - GCC (GNU Compiler Collection)

# Como rodar
Clone o repositório e compile o projeto:
 ```bash
git clone https://github.com/SlendyGamer/Mini-Powershell.git
cd Mini-Powershell
make
./mini-powershell
```

# Estrutura do projeto
```makefile
Mini-Powershell/
├──  Makefile/                #Arquivo para compilar todos os arquivos
├──  builtins.c/builtins.h    #Codigo de comandos internos, como cd e pwd
├──  external.c/external.h    #Codigo de comandos externos
├──  main.c                   #Codigo principal do shell
├──  minishell                #executavel
├──  shell.c/shell.h          #Codigo do shell em si, onde ele é executado
├──  sum.c                    #Codigo simples que faz a soma de dois valores
├──  utils.c/utils.h          #Codigo de funções auxiliares
```
