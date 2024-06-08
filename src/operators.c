/*
** EPITECH PROJECT, 2024
** B-PSU-200-MPL-2-1-minishell2-matys.laguerre
** File description:
** operations
*/

#include "my.h"
#include <fcntl.h>
#include <errno.h>

static void print_error_stdout(char *output_file)
{
    if (errno == EACCES) {
        my_puterror(output_file);
        my_puterror(": Permission denied.\n");
    }
    if (errno == EISDIR) {
        my_puterror(output_file);
        my_puterror(": Is a directory.\n");
    }
    if (errno == ENOENT) {
        my_puterror(output_file);
        my_puterror(": No such file or directory.\n");
    }
}

void execute_redirect_stdout(shell_t *Shell, tree_t *left, tree_t *right)
{
    char *output_file = right->value;
    int saved_stdout;
    int output_fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0666);

    if (output_fd == -1) {
        print_error_stdout(output_file);
        Shell->exit_status = 1;
        return;
    }
    saved_stdout = dup(STDOUT_FILENO);
    dup2(output_fd, STDOUT_FILENO);
    execute_tree(Shell, left);
    dup2(saved_stdout, STDOUT_FILENO);
    close(output_fd);
}

void execute_redirect_stdout_append(shell_t *Shell, tree_t *left,
    tree_t *right)
{
    char *output_file = right->value;
    int saved_stdout;
    int output_fd = open(output_file, O_WRONLY | O_CREAT | O_APPEND, 0666);

    if (output_fd == -1) {
        print_error_stdout(output_file);
        Shell->exit_status = 1;
        return;
    }
    saved_stdout = dup(STDOUT_FILENO);
    dup2(output_fd, STDOUT_FILENO);
    execute_tree(Shell, left);
    dup2(saved_stdout, STDOUT_FILENO);
    close(output_fd);
}

void execute_redirect_stdin(shell_t *Shell, tree_t *left, tree_t *right)
{
    char *input_file = right->value;
    int saved_stdin;
    char **input_clean = split_words(input_file, " \t\n\r", 1);
    int input_fd;

    input_file = input_clean[0];
    input_fd = open(input_file, O_RDONLY);
    if (input_fd == -1) {
        perror("open");
        Shell->exit_status = 1;
        return;
    }
    saved_stdin = dup(STDIN_FILENO);
    dup2(input_fd, STDIN_FILENO);
    execute_tree(Shell, left);
    dup2(saved_stdin, STDIN_FILENO);
    close(input_fd);
}

static void add_line(list_char_t **list, char *line)
{
    list_char_t *new = malloc(sizeof(list_char_t));
    list_char_t *current = *list;

    new->data = strdup(line);
    new->next = NULL;
    while (current != NULL && current->next != NULL)
        current = current->next;
    if (current != NULL)
        current->next = new;
    else
        *list = new;
}

static list_char_t *get_my_lines(char *delimiter)
{
    char *input = NULL;
    size_t bufsize = 0;
    list_char_t *lines = NULL;
    size_t len;

    while (getline(&input, &bufsize, stdin) != -1) {
        len = strlen(input);
        if (len > 0 && input[len - 1] == '\n')
            input[len - 1] = '\0';
        if (strcmp(input, delimiter) == 0)
            break;
        add_line(&lines, input);
    }
    free(input);
    return lines;
}

static void write_lines(list_char_t *lines, int pipefd[2])
{
    list_char_t *current = lines;

    while (current != NULL) {
        write(pipefd[1], current->data, strlen(current->data));
        write(pipefd[1], "\n", 1);
        current = current->next;
    }
}

void execute_here_doc(shell_t *Shell, tree_t *left, tree_t *right)
{
    list_char_t *lines = get_my_lines(right->value);
    int pipefd[2];
    list_char_t *current;
    list_char_t *next;

    pipe(pipefd);
    dup2(pipefd[0], STDIN_FILENO);
    close(pipefd[0]);
    write_lines(lines, pipefd);
    close(pipefd[1]);
    execute_tree(Shell, left);
    dup2(STDIN_FILENO, pipefd[0]);
    close(pipefd[0]);
    current = lines;
    while (current != NULL) {
        next = current->next;
        free(current->data);
        free(current);
        current = next;
    }
}

void execute_semicolon(shell_t *Shell, tree_t *left, tree_t *right)
{
    execute_tree(Shell, left);
    execute_tree(Shell, right);
}
