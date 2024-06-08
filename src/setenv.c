/*
** EPITECH PROJECT, 2024
** B-PSU-200-MPL-2-1-minishell1-matys.laguerre
** File description:
** envs
*/

#include "my.h"

static int is_variable(shell_t *Shell, char *name)
{
    list_t *current = Shell->env;

    while (current != NULL) {
        if (strncmp(name, current->name, strlen(name)) == 0) {
            return 1;
        }
        current = current->next;
    }
    return 0;
}

static int get_len(shell_t *Shell, char *value)
{
    int i = 0;
    int len = 0;
    int var = 0;

    while (value[i] != '\0') {
        if (value[i] == '$' && is_variable(Shell, &value[i + 1])) {
            len += strlen(get_env_value(Shell->env, &value[i + 1]));
            var = 1;
        }
        i++;
        len++;
    }
    if (var == 0)
        return -1;
    return len;
}

static char *get_new_value(shell_t *Shell, char *value, int len)
{
    int i = 0;
    char *new_value = malloc(len + 1);

    if (new_value == NULL)
        return NULL;
    new_value[0] = '\0';
    while (value[i] != '\0') {
        if (value[i] == '$' && is_variable(Shell, &value[i + 1])) {
            strcat(new_value, get_env_value(Shell->env, &value[i + 1]));
            i += strlen(&value[i + 1]);
        } else {
            new_value[i] = value[i];
            new_value[i + 1] = '\0';
        }
        i++;
    }
    return new_value;
}

// Check if $ is in the value and if the variable exists
static char *change_value(shell_t *Shell, char *value)
{
    int len;
    char *new_value = NULL;

    if (value[0] == '\0')
        return value;
    len = get_len(Shell, value);
    if (len == -1)
        return value;
    new_value = get_new_value(Shell, value, len);
    free(value);
    return new_value;
}

static int my_isalnum(char c)
{
    int res = 0;

    if (c >= '0' && c <= '9')
        res = 1;
    if (c >= 'A' && c <= 'Z')
        res = 1;
    if (c >= 'a' && c <= 'z')
        res = 1;
    if (c == '_')
        res = 1;
    return res;
}

int my_isletter(char c)
{
    if (c >= 'A' && c <= 'Z')
        return 1;
    if (c >= 'a' && c <= 'z')
        return 1;
    return 0;
}

static void free_value(char *value)
{
    if (value)
        free(value);
}

static int check_name(shell_t *Shell, char *name, char *value)
{
    const char msg[] = "setenv: Variable name must contain "
        "alphanumeric characters.\n";

    for (int i = 0; name[i] != '\0'; i++) {
        if (!my_isalnum(name[i])) {
            write(2, msg, sizeof(msg) - 1);
            Shell->exit_status = 0;
            free(name);
            free_value(value);
            Shell->exit_status = 1;
            return 1;
        }
    }
    return 0;
}

static void my_setenv(shell_t *Shell)
{
    char *name = strdup(Shell->args[1]);
    char *value = strdup((Shell->args[2] == NULL) ? "\0" : Shell->args[2]);
    list_t *current = Shell->env;

    if (check_name(Shell, name, value))
        return;
    value = change_value(Shell, value);
    while (current != NULL) {
        if (strncmp(name, current->name, strlen(name)) == 0 &&
            current->name[strlen(name)] == '\0') {
            free(current->value);
            current->value = value;
            free(name);
            Shell->exit_status = 0;
            return;
        }
        current = current->next;
    }
    add_node(&Shell->env, name, value, NULL);
    Shell->exit_status = 0;
}

int fcts_setenv(shell_t *Shell)
{
    if (Shell->args[1] == NULL) {
        fcts_env(Shell);
        Shell->exit_status = 0;
    } else if (Shell->args[2] != NULL && Shell->args[3] != NULL) {
        write(2, "setenv: Too many arguments.\n", 28);
        Shell->exit_status = 1;
    } else {
        my_setenv(Shell);
    }
    return 1;
}
