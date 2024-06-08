/*
** EPITECH PROJECT, 2024
** delivery
** File description:
** fcts_shell
*/

#include "my.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

char *get_command(shell_t *Shell)
{
    char *command = NULL;
    size_t bufsize = 0;
    ssize_t len;

    do {
        if (isatty(STDIN_FILENO)) {
            display_prompt(Shell);
            len = handle_input(&command, &bufsize, stdin, Shell);
        } else
            len = getline(&command, &bufsize, stdin);
        if (len == -1) {
            free(command);
            Shell->status = 0;
            return NULL;
        }
        if (len > 0 && command[len - 1] == '\n') {
            command[len - 1] = '\0';
        }
    } while (len <= 1);
    return command;
}

// Processus parent
static void check_segfault(shell_t *Shell, int status)
{
    int termsig = WTERMSIG(status);

    if (termsig == SIGSEGV) {
        my_puterror("Segmentation fault");
        if (WCOREDUMP(status))
            my_puterror(" (core dumped)");
        my_puterror("\n");
        Shell->exit_status = 139;
    }
    if (termsig == SIGPIPE)
        Shell->exit_status = 141;
    if (termsig == SIGFPE) {
        my_puterror("Floating exception");
        if (WCOREDUMP(status))
            my_puterror(" (core dumped)");
        my_puterror("\n");
        Shell->exit_status = 136;
    }
}

static int parent_process(shell_t *Shell, pid_t pid)
{
    int status;

    waitpid(pid, &status, WUNTRACED);
    if (WIFEXITED(status))
        Shell->exit_status = WEXITSTATUS(status);
    else if (WIFSIGNALED(status))
        check_segfault(Shell, status);
    return 1;
}

void extern_command(shell_t *Shell)
{
    pid_t pid = fork();

    if (pid == 0) {
        child_process(Shell);
        exit(Shell->exit_status);
    } else if (pid < 0)
        perror(Shell->args[0]);
    else
        parent_process(Shell, pid);
}

// Exécutez une commande simple
int execute_command(shell_t *Shell)
{
    command_t commandes[] = {{"cd", fcts_cd},
    {"setenv", fcts_setenv}, {"unsetenv", fcts_unsetenv},
    {"alias", fcts_alias}, {"unalias", fcts_unalias},
    {"set", fcts_set}, {"unset", fcts_unset}, {"env", fcts_env},
    {"exit", fcts_exit}, {"history", fcts_history}, {NULL, NULL}};

    if (Shell->args[0] == NULL)
        return 1;
    for (int i = 0; commandes[i].name != NULL; i++) {
        exec_alias(Shell);
        if (strcmp(Shell->args[0], commandes[i].name) == 0)
            return commandes[i].func(Shell);
    }
    extern_command(Shell);
    return 1;
}
