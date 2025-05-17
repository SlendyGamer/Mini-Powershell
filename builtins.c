#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h> 
#include "builtins.h"
#include "utils.h"
#include <stdbool.h>

int is_builtin(char *cmd) 
{
    return (strcmp(cmd, "cd") == 0 || strcmp(cmd, "exit") == 0 || strcmp(cmd, "pwd") == 0 || strcmp(cmd, "ls") == 0); //confere se o que foi digitado eh uma destas funcoes
}

void execute_builtin(char **args) //executa funcoes acima
{
    if (strcmp(args[0], "cd") == 0) 
    {
        if (args[1] == NULL) //se for digitado apenas "cd" e sem nenhum argumento, da erro
        {
            fprintf(stderr, "cd: missing argument\n");
        } 
        else 
        {
            if (chdir(args[1]) != 0) //tenta mudar diretorio para o argumento, se retornar -1, houve erro
            {
                perror("cd");
            }
        }
    }
    else if (strcmp(args[0], "exit") == 0) 
    {
        exit(0); //finaliza powershell
    }
    else if (strcmp(args[0], "pwd") == 0) 
    {
        char cwd[1024];
        if (getcwd(cwd, sizeof(cwd)) != NULL)  //pega a localização atual do diretorio
        {
            printf("%s\n", cwd);
        } 
        else 
        {
            perror("pwd");
        }
    }
    else if (strcmp(args[0], "ls") == 0)
    {
        DIR *d;
        struct dirent *dir;
        int contador = 0;
        FileInfo info;
        int opt;
        optind = 1; //reset das variaveis globais do getopt
        opterr = 0;
        int argc = 0;

        bool flag_a = false;
        bool flag_l = false;

        
        while(args[argc] != NULL){argc++;}  


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
                if (strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0) //pula a indicaçao de diretorio atual e pai
                  continue;
                if (!flag_a && dir->d_name[0] == '.') //Pula o arquivo invisivel se n tiver a flag
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
        }
        else
        {
            perror("ls");
        }
    }
}
