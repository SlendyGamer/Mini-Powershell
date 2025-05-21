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
    //char **args = split_line(linha, " \t\n"); //divide a linhad e comando digitada pelos espacos
    //char *temp = args[1];
    //printf("%s", temp);
  
    char **comandos = split_line(linha, "&\n"); //vai seprar os camandos por &

    for (int i = 0; comandos[i] != NULL; i++) { //para cada comando separado vai fazer um processo
       
        char **args = split_line(comandos[i], " \t\n"); // divir em palavras
        if (args[0] == NULL)    //se a primeira palavra for nula, libera args e nao faz nada
        {
          free_tokens(args);
          exit(0);
        }
      if(strcmp(args[0],"exit") == 0)
    {
      free_tokens(args);
      free_tokens(comandos);
      exit(0);
    }
     
      pid_t pid = fork(); //processo filho

      if (pid == 0) { //vai executar o comando separado

      int stdout_redirected = 0;  //salva o estado anterior
      int saved_stdout = -1;      //salva o stdout em si
    
      if (outputRedirect(args)) //Se houver o >, vai redirecionar
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
            exit(1);
          }
        }
      }


      if (is_builtin(args[0])) { // v c é comando interno
        execute_builtin(args);
        if (stdout_redirected) {
            dup2(saved_stdout, STDOUT_FILENO);
            close(saved_stdout);
        }
        free_tokens(args);
        exit(0);
    }

      if(is_external(args[0])) //v c é comando externo
      {
        execute_external(args);
      } else {
        execvp(args[0], args);
        perror("execvp");
      }
      
    
      if(stdout_redirected) //se ocorreu o redirecionamento, retorna para o stdout padrao
      {
        dup2(saved_stdout, STDOUT_FILENO);
        close(saved_stdout);
      }

     free_tokens(args);
     exit(1);
    }
  }
  while(wait(NULL)>0);
  printf("teste depois do while");
  free_tokens(comandos);
}
