/*
** EPITECH PROJECT, 2024
** delivery
** File description:
** builtins
*/
#include <stdlib.h>
#include <stdio.h>
#include "my.h"

char *get_env_value(list_t *env, char *name)
{
    list_t *current = env;

    while (current != NULL) {
        if (strncmp(name, current->name, strlen(name)) == 0) {
            return current->value;
        }
        current = current->next;
    }
    return NULL;
}

// Unsetenv
static void my_unsetenv(shell_t *Shell)
{
    list_t *current = Shell->env;
    char *name = Shell->args[1];
    list_t *previous = current;

    while (current != NULL) {
        if (strncmp(name, current->name, strlen(name)) == 0 &&
            current->name[strlen(name)] == '\0') {
            previous->next = current->next;
            free(current->name);
            free(current->value);
            free(current);
            return;
        }
        previous = current;
        current = current->next;
    }
}

int fcts_unsetenv(shell_t *Shell)
{
    if (Shell->args[1] == NULL) {
        write(2, "unsetenv: Too few arguments.\n", 29);
        Shell->exit_status = 1;
    } else {
        my_unsetenv(Shell);
        Shell->exit_status = 0;
    }
    return 1;
}

int fcts_exit(shell_t *Shell)
{
    Shell->status = 0;
    return 0;
}

int fcts_env(shell_t *Shell)
{
    list_t *current_env = Shell->env;

    while (current_env) {
        printf("%s=%s\n", current_env->name, current_env->value);
        current_env = current_env->next;
    }
    Shell->exit_status = 0;
    return 1;
}
