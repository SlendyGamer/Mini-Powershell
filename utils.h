#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

char **split_line(char *line, const char *delim);
char **split_linee(const char *line);
void free_tokens(char **tokens);
void freee_tokens(char **tokens);
int check_path_buffer(char *destino, size_t tamanho, const char *prefixo, const char *comando);

typedef struct FileInfo {
    char permissions[11];      // Ex: -rw-r--r--
    nlink_t links;             // Número de links
    char owner[64]; // Nome do dono
    char group[64]; // Nome do grupo
    off_t size;                // Tamanho do arquivo
    char mod_time[64];   // Data de modificação (ex: "May 14 13:42")
    char name[256];   // Nome do arquivo
} FileInfo;

FileInfo getFileInfo(const char *filename);
void getPermissions(mode_t mode, char *perm_str);
int outputRedirect(char **args);
char *getRedirectFilename(char **args);
void removeRedirectTokens(char **args);
int redirectStdout(const char *filename);

#endif // UTILS_H_INCLUDED
