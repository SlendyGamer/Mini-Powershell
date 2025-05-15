#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include <stdio.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <string.h>

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

void free_tokens(char **tokens) 
{
    free(tokens);
}

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

