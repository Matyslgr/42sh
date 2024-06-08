/*
** EPITECH PROJECT, 2024
** Backticks
** File description:
** backticks
*/

#include "my.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/ioctl.h>

bool *backtic(void)
{
    static bool backticks = false;

    return &backticks;
}

static int parent_backticks(int *pid, int *pipefd, shell_t *shell, int *i)
{
    int nbytes = 0;
    char *str = NULL;

    waitpid(*pid, NULL, 0);
    ioctl(pipefd[0], FIONREAD, &nbytes);
    if (nbytes > 0) {
        str = malloc(sizeof(char) * (nbytes));
        read(pipefd[0], str, nbytes);
        close(pipefd[0]);
        str[nbytes - 1] = '\0';
    }
    free(shell->args[*i]);
    for (int i = 0; str && str[i] != '\0'; i++)
        str[i] = (str[i] == '\n') ? ' ' : str[i];
    shell->args[*i] = (nbytes > 0) ? str : NULL;
    if (shell->args[*i] == NULL && shell->args[*i + 1] != NULL) {
        shell->args[*i] = shell->args[*i + 1];
        *i -= 1;
    }
    return 0;
}

static char *manage_args(shell_t *shell, int *i)
{
    for (; shell->args[*i][0] &&
    shell->args[*i][0] != '`'; shell->args[*i]++) {
        printf("%c", shell->args[*i][0]);
        fflush(stdout);
    }
    shell->args[*i]++;
    return shell->args[*i];
}

int manage_backticks(int *pipefd, shell_t *shell, int *i, char *str)
{
    static int pid = 0;
    char *tmp = NULL;
    int a = 0;

    pipe(pipefd);
    pid = fork();
    if (pid == 0) {
        shell->args[*i] = manage_args(shell, i);
        for (; shell->args[*i][a] && shell->args[*i][a] != '`'; a++);
        tmp = (shell->args[*i][a] && shell->args[*i][a + 1] != '\0') ?
        strdup(&shell->args[*i][a + 1]) : tmp;
        shell->args[*i][a] = '\0';
        dup2(pipefd[1], STDOUT_FILENO);
        str = strdup(shell->args[*i]);
        shell->args = split_words(shell->args[*i], " \t\n\r", 1);
        execute_command_line(shell, str);
        a = (tmp != NULL) ? printf("%s", tmp) : a;
        exit(0);
    }
    return parent_backticks(&pid, pipefd, shell, i);
}

int find_backticks(char *args)
{
    for (int i = 0; args[i] != '\0'; i++) {
        if (args[i] == '`')
            return 1;
    }
    return 0;
}

int backticks(tree_t *tree, shell_t *shell)
{
    int *pipefd = malloc(sizeof(int) * 2);
    char *str = NULL;
    int find = 0;

    for (int i = 0; tree->value[i] != '\0'; i++) {
        if (tree->value[i] == '`')
            find = 1;
    }
    if (find == 0)
        return 0;
    for (int i = 0; shell->args[i] != NULL; i++) {
        if (find_backticks(shell->args[i]) == 1)
            manage_backticks(pipefd, shell, &i, str);
    }
    return 0;
}
