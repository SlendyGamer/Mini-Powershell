#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

char **split_line(char *line, const char *delim);
void free_tokens(char **tokens);

#endif // UTILS_H_INCLUDED
