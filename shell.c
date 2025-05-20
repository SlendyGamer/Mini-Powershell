#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "shell.h"
#include "builtins.h"
#include "external.h"
#include "utils.h"

void execute_cmd(char *linha)
{
    char **args = split_line(linha, " \t\n"); //divide a linhad e comando digitada pelos espacos
    //char *temp = args[1];
    //printf("%s", temp);
    if (args[0] == NULL)    //se a primeira palavra for nula, libera args e nao faz nada
    {
        free_tokens(args);
        return;
    }

    int stdout_redirected = 0;  //salva o estado anterior
    int saved_stdout = -1;      //salva o stdout em si
    
    if (outputRedirect(args))
    {
      char* filename = getRedirectFilename(args);
      if(filename != NULL)
      {
        saved_stdout = dup(STDOUT_FILENO);
        if (redirectStdout(filename) == 0)
        {
          stdout_redirected = 1;
          removeRedirectTokens(args);
        }else{
          fprintf(stderr, "Erro ao direcionar saida\n");
          free_tokens(args);
          return;
        }
      }
    }
    //printf("printar\n");
    if (is_builtin(args[0])) //se for cd, exit ou pwd, retorna 1
    {
      execute_builtin(args);  //redireciona para os arquivos builtins para execucao
    }
    else if(is_external(args[0]))
    {
      execute_external(args);
    }
    else //se o comando for qualquer outro, cria um processo para executa-lo
    {
        pid_t pid = fork();
        if (pid == 0)
        {
            execvp(args[0], args); //executa o comando qualquer
            perror("execvp");
            exit(EXIT_FAILURE);
        }
        else
        {
            wait(NULL);
        }
    }
    
    if(stdout_redirected) //se ocorreu o redirecionamento, retorna para o stdout padrao
    {
      dup2(saved_stdout, STDOUT_FILENO);
      close(saved_stdout);
    }

    free_tokens(args);
}
