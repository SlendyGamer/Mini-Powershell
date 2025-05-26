#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h> 
#include "external.h"
#include "utils.h"
#include <stdbool.h>
#include <sys/wait.h>

int is_external(char *cmd) 
{
    return 1; //confere se o que foi digitado eh uma destas funcoes
}

void execute_external(char **args)
{
  if (strcmp(args[0],"ls") == 0)
  {
    DIR *d;
    struct dirent *dir;
    int contador = 0;
    FileInfo info;
    int opt;

    int argc = 0;
    bool flag_a = false;
    bool flag_l = false;

    while(args[argc] != NULL){argc++;}  
    
    optind = 0; //reset das variaveis globais do getopt
    opterr = 0;
    optarg = NULL;
    optopt = 0;

    while((opt = getopt(argc, args, "al")) != -1)
    {
      switch(opt)
      {
        case 'a':
          flag_a = true;
          break;
        case 'l':
          flag_l = true;
          break;
        default:
          fprintf(stderr, "Uso: ls [-a/l] [diretorio]\n");
          return;
      }
    }
    
        
    char* path= ".";
    if(optind < argc){
      path=args[optind];
    }

    d=opendir(path);

    if (d)
    {
      while ((dir = readdir(d)) != NULL)
      {
        if (!flag_a && dir->d_name[0] == '.') //Pula o arquivo invisivel se n tiver a flag
          continue;

        if (strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0) //pula a indicaçao de diretorio atual e pai
          continue;

        if(flag_l)
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
        }else{
          printf("%s\t",dir->d_name);
          contador++;
          if(contador ==5)
          {
            printf("\n");
            contador=0;
          }
        }
      }
      if(contador>0) printf("\n");
      closedir(d);
    }
    else
    {
      perror("ls");
    }
  }else if(strcmp(args[0],"cat") == 0)
  {
    FILE *fptr = fopen(args[1],"r");
    if(fptr == NULL)
    {
      printf("Erro ao abrir o arquivo ou arquivo nao existente.");
    }

    char buff[100];
    size_t n;
    while((n = fread(buff, sizeof(char), sizeof(buff) -1, fptr)) > 0)
    {
      buff[n]='\0';
      printf("%s",buff);
    }
    fclose(fptr);
  }
  else{
    char *path_env = getenv("PATH");

      if (!path_env) {                                                          // verifica se a variavel "path" eh definida 
          fprintf(stderr, "Erro: variavel de ambiente PATH nao definida.\n");   // se nao for definida = ERRO
          return;
      }

      char *path_copy = strdup(path_env);                                       // copia a string para "path copy"
      char *dir = strtok(path_copy, ";");                                       // divide a string "path_copy" em tokens separados por ";"

      char fullpath[1024];                                                  
      int found = 0;

      while (dir != NULL) {                                                     // verifica todos os diretorio na 
          snprintf(fullpath, sizeof(fullpath), "%s/%s", dir, args[0]);          // monta o caminho completo do executavel, combinando o diretorio atual com o comando

          if (access(fullpath, X_OK) == 0) {                                    //verfica se o arquivo existe em determinado caminho
              found = 1;
              break;
          }

          dir = strtok(NULL, ";");                                              // vai para o proximo diretorio separado por ";"
      }

      if (!found) {                                                             // Se nao achou no PATH, tenta no diretorio atual
          snprintf(fullpath, sizeof(fullpath), "./%s", args[0]);                // monta o caminho completo do executavel, combinando o diretorio atual com o comando

          if (access(fullpath, X_OK) == 0) {                                    //verfica se o arquivo existe em determinado caminho
              found = 1;
          }
      }

      if (!found) {
          fprintf(stderr, "Erro: comando '%s' nao encontrado.\n", args[0]);     // se o executavel nao foi encontrado em nenhum dos caminhos PATh, nem no diretorio atual = ERRO
          free(path_copy);
          return;
      }

      pid_t pid = fork();                                                        // executa o programa em um novo processo

      if (pid < 0) { 
          perror("fork");                                                        // se retornar NEGATIVO = ERRO
          free(path_copy);                                                       // libera a memoria alocada
          return;
      }

      if (pid == 0) {
          execv(fullpath, args);                                                 // executa o executavel
          perror("execv");                                                       // se execucao falhar = ERRO
          exit(EXIT_FAILURE);                                                    // encerra o processo com mensagem de ERRO
      }else {

        int status;                                                               
        waitpid(pid, &status, 0);                                                // aguarda o processo filho terminar e recebe seu "status"

        if (WIFEXITED(status)) {                                                 // entra no if se executou NORMALMENTE
            printf("Codigo de saida: %d", WEXITSTATUS(status));                // imprime ZERO se deu tudo CERTO, imprime UM se deu ERRO
        }
    }

      free(path_copy);                                                           // libera a memoria alocada
  }
    
}