/*
** EPITECH PROJECT, 2024
** B-PSU-200-MPL-2-1-minishell2-matys.laguerre
** File description:
** execute_pipe
*/

#include "my.h"

static pid_t left_node(shell_t *Shell, tree_t *left, int pipe_fd[2])
{
    pid_t pid_left = fork();

    if (pid_left == -1) {
        perror("fork");
        close(pipe_fd[0]);
        close(pipe_fd[1]);
        Shell->exit_status = 1;
        return -1;
    } else if (pid_left == 0) {
        close(pipe_fd[0]);
        dup2(pipe_fd[1], STDOUT_FILENO);
        close(pipe_fd[1]);
        execute_tree(Shell, left);
        exit(Shell->exit_status);
    }
    return pid_left;
}

static void check_right_node_exit_or_cd(shell_t *Shell, tree_t *right)
{
    char **args = split_words(right->value, " \t\n\r", 1);

    if (args[0] && !strcmp(args[0], "exit"))
        Shell->status = 0;
    if (args[0] && !strcmp(args[0], "cd")) {
        Shell->args = args;
        fcts_cd(Shell);
    }
    for (int i = 0; args[i] != NULL; i++)
        free(args[i]);
    free(args);
}

static pid_t right_node(shell_t *Shell, tree_t *right, int pipe_fd[2])
{
    pid_t pid_right = fork();

    if (pid_right == -1) {
        perror("fork");
        close(pipe_fd[0]);
        close(pipe_fd[1]);
        Shell->exit_status = 1;
        return -1;
    } else if (pid_right == 0) {
        close(pipe_fd[1]);
        dup2(pipe_fd[0], STDIN_FILENO);
        close(pipe_fd[0]);
        execute_tree(Shell, right);
        exit(Shell->exit_status);
    } else
        check_right_node_exit_or_cd(Shell, right);
    return pid_right;
}

static void get_status_of_nodes(shell_t *Shell, pid_t pid_left,
    pid_t pid_right)
{
    int left_status;
    int right_status;

    waitpid(pid_left, &left_status, 0);
    waitpid(pid_right, &right_status, 0);
    if (WIFEXITED(left_status) && WEXITSTATUS(left_status) != 0) {
        Shell->exit_status = WEXITSTATUS(left_status);
        return;
    }
    if (WIFEXITED(right_status) && WEXITSTATUS(right_status) != 0) {
        Shell->exit_status = WEXITSTATUS(right_status);
        return;
    }
    Shell->exit_status = 0;
}

void execute_pipe(shell_t *Shell, tree_t *left, tree_t *right)
{
    int pipe_fd[2];
    pid_t pid_right;
    pid_t pid_left;

    if (pipe(pipe_fd) == -1) {
        perror("pipe");
        Shell->exit_status = 1;
        return;
    }
    pid_left = left_node(Shell, left, pipe_fd);
    pid_right = right_node(Shell, right, pipe_fd);
    close(pipe_fd[0]);
    close(pipe_fd[1]);
    if (pid_left == -1 || pid_right == -1)
        return;
    get_status_of_nodes(Shell, pid_left, pid_right);
}
