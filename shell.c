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
    char **comandos = split_line(linha, "&\n");   //Separa o comando pelos &

    for (int i = 0; comandos[i] != NULL; i++) {   //loop principal

        char **pipe_cmds = split_line(comandos[i], "|");    //separa os comandos por pipe, se tiver
        int n = 0;    //contador da quantidade de pipes
        while (pipe_cmds[n] != NULL) n++;

        int flag_pipe= (n > 1);   //flag do pipe 

        int in_fd = 0; // entrada padrão (stdin)
        int pipefd[2];

        for (int j = 0; j < n; j++) {
          
            char **args = split_line(pipe_cmds[j], " \t\n");    //separa cada comando por palavras
            if (args[0] == NULL) {    //se aconteceu algum erro ou nao tiver o comando ele finaliza
              free_tokens(args);
              continue;
            }
            if(strcmp(args[0], "exit")==0)    //se o comando for exit, ele da free em tudo e finaliza
            {
              free_tokens(args);
              free_tokens(pipe_cmds);
              free_tokens(comandos);
              exit(0);
            }
            
            if(!flag_pipe && is_builtin(args[0]))   //se nao tiver pipe e for builtin, executa no processo princial
            {
              execute_builtin(args);
              free_tokens(args);
              continue;
            }
            
            if(flag_pipe && j < n-1)  //se for comando com pipe e nao for ultimo, cria o pipr
            {
              if(pipe(pipefd) == -1)
              {
                perror("pipe");
                exit(1);
              }
            }

            pid_t pid = fork();     //cria o filho

            if (pid == 0) {
                // filho redireciona entrada, se não for o primeiro comando
                if (in_fd != 0) {
                    dup2(in_fd, STDIN_FILENO);
                    close(in_fd);
                }

                // redireciona saída, se tiver pipe depois
                if (flag_pipe && j < n - 1) {
                    dup2(pipefd[1], STDOUT_FILENO);
                    close(pipefd[0]);
                    close(pipefd[1]);
                }


                 // redirecionamento de saída se tiver o '>'
                if (!flag_pipe || (flag_pipe && j == n-1)) {
                    int stdout_redirected = 0;
                    int saved_stdout = -1;

                    if (outputRedirect(args)) {
                        char* filename = getRedirectFilename(args);
                        if (filename != NULL) {
                            saved_stdout = dup(STDOUT_FILENO);
                            if (redirectStdout(filename) == 0) {
                                stdout_redirected = 1;
                                removeRedirectTokens(args);
                            } else {
                                fprintf(stderr, "Erro ao direcionar saida\n");
                                free_tokens(args);
                                exit(1);
                            }
                        }
                    }

                    if (is_builtin(args[0])) {  //se o comando for no builtin, executa ele com o tratamento do stdout
                        execute_builtin(args);
                        if (stdout_redirected) {
                            dup2(saved_stdout, STDOUT_FILENO);
                            close(saved_stdout);
                        }
                        free_tokens(args);
                        exit(0);
                    } else if (is_external(args[0])) {    //verifica se tem externo, se tiver executa, se nao tiver ele manda para o terminal principal, nao para nosso shell
                        execute_external(args);
                    } else {
                        execvp(args[0], args);
                        perror("execvp");
                    }

                    if (stdout_redirected) {    //se tiver redirecionado ele retorna o stdout para o padrao de antes
                        dup2(saved_stdout, STDOUT_FILENO);
                        close(saved_stdout);
                    }

                    free_tokens(args);
                    exit(1);
                } else {
                    // Para os comandos no meio do pipe, executa normal (builtin ou external)
                    if (is_builtin(args[0])) {
                        execute_builtin(args);
                        free_tokens(args);
                        exit(0);
                    }else if (is_external(args[0])) {
                        execute_external(args);
                    } else {
                        execvp(args[0], args);
                        perror("execvp");
                    }
                    free_tokens(args);
                    exit(1);
                }
            } else if (pid > 0) {
                // pai fecha os descritores que não usa e prepara entrada para próximo comando
                if (in_fd != 0)
                    close(in_fd);

                if (flag_pipe && j < n - 1) {
                    close(pipefd[1]);
                    in_fd = pipefd[0];
                }
            } else {
                perror("fork");
                exit(1);
            }
        }

        if (in_fd != 0)   //fecha o restante
            close(in_fd);

        free_tokens(pipe_cmds);   //fecha o pipe
    }

    while(wait(NULL) > 0);    //espera todos processos finalizarem e da free
    free_tokens(comandos);
}