/*
** EPITECH PROJECT, 2024
** B-PSU-200-MPL-2-1-minishell1-matys.laguerre
** File description:
** child_process
*/
#include "my.h"
#include <errno.h>

static void free_directories(char **directories)
{
    if (directories == NULL)
        return;
    for (int i = 0; directories[i] != NULL; i++) {
        free(directories[i]);
    }
    free(directories);
}

static char *command_is_bin(char *command)
{
    for (int i = 0; command[i] != '\0'; i++) {
        if (command[i] == ' ')
            return strndup(command, i);
    }
    return command;
}

static char *get_bin_path(shell_t *Shell)
{
    char *command = command_is_bin(Shell->args[0]);
    char *path = get_env_value(Shell->env, "PATH");
    char **directories = NULL;
    char *bin_path = NULL;

    directories = my_str_to_word_array(path, ":");
    for (int i = 0; directories && directories[i] != NULL; i++) {
        bin_path = malloc(sizeof(char) * (strlen(directories[i]) +
            strlen(command) + 2));
        bin_path = strcpy(bin_path, directories[i]);
        strcat(bin_path, "/");
        strcat(bin_path, command);
        if (access(bin_path, F_OK) == 0) {
            free_directories(directories);
            return bin_path;
        }
        free(bin_path);
    }
    free_directories(directories);
    return NULL;
}

static int my_list_size(list_t *list)
{
    int i = 0;

    while (list != NULL) {
        list = list->next;
        i++;
    }
    return i;
}

static char **get_tab_env(list_t *env)
{
    int i = 0;
    char **tab_env = malloc(sizeof(char *) * (my_list_size(env) + 1));
    list_t *current = env;

    while (current != NULL) {
        tab_env[i] = malloc(sizeof(char) * (strlen(current->name) +
            strlen(current->value) + 2));
        strcpy(tab_env[i], current->name);
        strcat(tab_env[i], "=");
        strcat(tab_env[i], current->value);
        current = current->next;
        i++;
    }
    tab_env[i] = NULL;
    return tab_env;
}

static void exec_process(shell_t *Shell, char *command, char **tab_env)
{
    if (execve(command, Shell->args, tab_env) == -1) {
        if (errno == ENOEXEC) {
            write(2, command, strlen(command));
            write(2, ": Exec format error. Wrong Architecture.\n", 41);
        }
        if (errno == EACCES) {
            write(2, command, strlen(command));
            write(2, ": Permission denied.\n", 21);
        }
        if (errno == ENOENT) {
            write(2, command, strlen(command));
            write(2, ": No such file or directory.\n", 29);
        }
        Shell->exit_status = 1;
    }
}

// Processus fils
static void free_bin_and_env(char *bin_path, char **tab_env)
{
    free(bin_path);
    for (int i = 0; tab_env[i] != NULL; i++) {
        free(tab_env[i]);
    }
    free(tab_env);
}

void child_process(shell_t *Shell)
{
    char *bin_path = get_bin_path(Shell);
    char **tab_env = get_tab_env(Shell->env);

    if (access(Shell->args[0], F_OK) == 0) {
        exec_process(Shell, Shell->args[0], tab_env);
    } else if (bin_path != NULL && access(bin_path, F_OK) == 0) {
        exec_process(Shell, bin_path, tab_env);
    } else {
        write(2, Shell->args[0], strlen(Shell->args[0]));
        write(2, ": Command not found.\n", 21);
        Shell->exit_status = 1;
    }
    free_bin_and_env(bin_path, tab_env);
}
