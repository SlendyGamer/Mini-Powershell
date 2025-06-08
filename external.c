#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h> 
#include "external.h"
#include "utils.h"
#include "builtins.h"
#include <stdbool.h>
#include <sys/wait.h>

int is_external(char *cmd) 
{
  return !is_builtin(cmd);
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
  else {
      extern char caminho_salvo[1024];
      char caminho_completo[1024];

      if (args[0][0] == '.' || args[0][0] == '/') {
        if (access(args[0], X_OK) == 0) {
            printf("[DEBUG] Executando (com caminho direto): %s\n", args[0]);
            execv(args[0], args);
            perror("execv");
            exit(EXIT_FAILURE);
        } else {
            fprintf(stderr, "Erro: arquivo '%s' não encontrado ou sem permissão de execução.\n", args[0]);
            exit(EXIT_FAILURE);
        }
      }
      
      if (strlen(caminho_salvo) > 0) {
        
        if (check_path_buffer(caminho_completo, sizeof(caminho_completo), caminho_salvo, args[0]) < 0) {
            fprintf(stderr, "Erro: caminho muito longo para o buffer.\n");
            return;
        }

          if (access(caminho_completo, X_OK) == 0) {

              fprintf(stderr, "comando '%s' ENCONTRADO via PATH %s resultado: \n", args[0], caminho_salvo);
              execv(caminho_completo, args);
              perror("execv");
              exit(EXIT_FAILURE);
          }else{

            fprintf(stderr, "comando '%s' NAO encontrado em PATH SALVO: %s\n", args[0], caminho_salvo);

          }
      }

      char *path_env = getenv("PATH");
    if (!path_env) {
        fprintf(stderr, "Erro: variável de ambiente PATH não definida.\n");
        return;
    }

    char *path_copy = strdup(path_env);
    char *dir = strtok(path_copy, ":");
    int found = 0;

    while (dir != NULL) {
      if (check_path_buffer(caminho_completo, sizeof(caminho_completo), dir, args[0]) < 0) {
            fprintf(stderr, "Erro: caminho gerado pelo PATH muito longo para o buffer.\n");
            dir = strtok(NULL, ":");
            continue;
        }
        if (access(caminho_completo, X_OK) == 0) {  // <-- testa se o comando é executável aqui
        found = 1;
        break;  // <-- comando encontrado, sai do loop
    }
        dir = strtok(NULL, ":");
    }

    if (found) {
        fprintf(stderr, "[DEBUG] comando '%s' encontrado no PATH do sistema.\n", args[0]);
        execv(caminho_completo, args);
        perror("execv");
        exit(EXIT_FAILURE);
    } else {
        fprintf(stderr, "Erro: comando '%s' não encontrado.\n", args[0]);
    }

    free(path_copy);
    
  }
}