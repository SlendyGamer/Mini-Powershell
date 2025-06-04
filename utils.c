#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include <stdio.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

char **split_line(char *line, const char *delim) 
{
    int bufsize = 64, position = 0;
    char **tokens = malloc(bufsize * sizeof(char*));
    char *token;

    token = strtok(line, delim);
    while (token != NULL) 
    {
        tokens[position++] = token;
        if (position >= bufsize) 
        {
            bufsize += 64;
            tokens = realloc(tokens, bufsize * sizeof(char*));
        }
        token = strtok(NULL, delim);
    }
    tokens[position] = NULL;
    return tokens;
}

char **split_linee(const char *line) {
    int bufsize = 64, position = 0;
    char **tokens = malloc(bufsize * sizeof(char*));
    const char *ptr = line;

    while (*ptr) {
        // Pula espaços
        while (*ptr == ' ' || *ptr == '\t' || *ptr == '\n') ptr++;
        if (*ptr == '\0') break;

        char *token = malloc(1024);
        int i = 0;

        if (*ptr == '"') {
            ptr++; // pula a primeira aspa
            while (*ptr && *ptr != '"') {
                token[i++] = *ptr++;
            }
            if (*ptr == '"') ptr++; // pula a última aspa
        } else {
            while (*ptr && *ptr != ' ' && *ptr != '\t' && *ptr != '\n') {
                token[i++] = *ptr++;
            }
        }
        token[i] = '\0';
        tokens[position++] = token;

        if (position >= bufsize) {
            bufsize += 64;
            tokens = realloc(tokens, bufsize * sizeof(char*));
        }
    }

    tokens[position] = NULL;

    if (position > 0 && strcmp(tokens[0], "grep") == 0) {//teste
        // verifica se já tem --color
        int has_color = 0;
        for (int i = 1; i < position; i++) {
            if (strstr(tokens[i], "--color") != NULL) {
                has_color = 1;
                break;
            }
        }

        if (!has_color) {
            // realoca para um espaço extra
            tokens = realloc(tokens, (position + 2) * sizeof(char*)); 
            // Move todos os argumentos para a frente para abrir espaço em tokens[1]
            for (int i = position; i > 0; i--) {
                tokens[i] = tokens[i - 1];
            }
            // Insere o --color=always em tokens[1]
            tokens[1] = strdup("--color=always");
            position++;
            tokens[position] = NULL;
        }
    }//teste

    return tokens;
} //teste


void free_tokens(char **tokens) 
{
    free(tokens);
}

void freee_tokens(char **tokens) 
{
    if (!tokens) return;
    for (int i = 0; tokens[i] != NULL; i++) {
        free(tokens[i]);  // libera cada string individual
    }
    free(tokens); // libera o vetor de ponteiros
} //teste

void getPermissions(mode_t mode, char *perm_str) {
    perm_str[0] = S_ISDIR(mode) ? 'd' :
                  S_ISLNK(mode) ? 'l' :
                  S_ISSOCK(mode) ? 's' :
                  S_ISCHR(mode) ? 'c' :
                  S_ISBLK(mode) ? 'b' :
                  S_ISFIFO(mode) ? 'p' : '-';

    perm_str[1] = (mode & S_IRUSR) ? 'r' : '-';
    perm_str[2] = (mode & S_IWUSR) ? 'w' : '-';
    perm_str[3] = (mode & S_IXUSR) ? 'x' : '-';
    perm_str[4] = (mode & S_IRGRP) ? 'r' : '-';
    perm_str[5] = (mode & S_IWGRP) ? 'w' : '-';
    perm_str[6] = (mode & S_IXGRP) ? 'x' : '-';
    perm_str[7] = (mode & S_IROTH) ? 'r' : '-';
    perm_str[8] = (mode & S_IWOTH) ? 'w' : '-';
    perm_str[9] = (mode & S_IXOTH) ? 'x' : '-';
    perm_str[10] = '\0';
}

int check_path_buffer(char *destino, size_t tamanho, const char *prefixo, const char *comando) {
    if (strlen(prefixo) + strlen(comando) + 2 > tamanho)
        return -1;
    snprintf(destino, tamanho, "%s/%s", prefixo, comando);
    return 0;
}

FileInfo getFileInfo(const char *filename)
{
    FileInfo info;
    struct stat st;
    struct passwd *pw;
    struct group *gr;

    //Inicia
    memset(&info, 0, sizeof(FileInfo));
    strncpy(info.name, filename, 256);

    if (stat(filename, &st) == -1)
    {
        perror("stat");
        strncpy(info.permissions, "??????????", 11);
        return info;
    }

    getPermissions(st.st_mode, info.permissions);
    info.links = st.st_nlink;

    pw = getpwuid(st.st_uid);
    strncpy(info.owner, pw ? pw->pw_name : "???", 64);

    gr = getgrgid(st.st_gid);
    strncpy(info.group, gr ? gr->gr_name : "???", 64);

    info.size = st.st_size;
    strftime(info.mod_time, 64, "%b %d %H:%M", localtime(&st.st_mtime));

    return info;
}

int outputRedirect(char **args)
{
  for(int i=0; args[i]!=NULL;i++)
  {
    if(strcmp(args[i],">") == 0)  return 1;
  }
  return 0;
}

char *getRedirectFilename(char **args)
{
  for(int i=0; args[i] != NULL; i++)
  {
    if(strcmp(args[i], ">") == 0 && args[i+1] != NULL)  return args[i+1];
  }
  return NULL;
}

void removeRedirectTokens(char **args)
{
  for(int i=0; args[i] != NULL; i++)
  {
    if(strcmp(args[i], ">") == 0 && args[i+1] != NULL)
    {
      args[i] = NULL;
      args[i+1] = NULL;
      break;
    }
  }
}

int redirectStdout(const char *filename)
{
  int fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
  if (fd < 0){
    perror("redirectStdout");
    return -1;
  }
  dup2(fd, STDOUT_FILENO);
  close(fd);
  return 0;
}