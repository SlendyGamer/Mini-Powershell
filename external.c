#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h> 
#include "external.h"
#include "utils.h"
#include <stdbool.h>

int is_external(char *cmd) 
{
    return (strcmp(cmd, "ls") == 0 || strcmp(cmd, "cat") == 0); //confere se o que foi digitado eh uma destas funcoes
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
}
