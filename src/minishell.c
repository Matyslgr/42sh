/*
** EPITECH PROJECT, 2024
** delivery
** File description:
** minishell
*/

#include "my.h"
#include <stdlib.h>

void add_node(list_t **list, char *name, char *value, char *alias)
{
    list_t *node = malloc(sizeof(list_t));
    list_t *tmp = *list;

    node->name = name;
    node->value = value;
    node->full_name = alias;
    node->oui = 0;
    node->next = NULL;
    if (*list == NULL) {
        *list = node;
        return;
    }
    while (tmp->next != NULL) {
        tmp = tmp->next;
    }
    tmp->next = node;
}

static char *get_name(char *env)
{
    char *name;
    int i = 0;

    while (env[i] != '=' && env[i] != '\0')
        i++;
    name = malloc(sizeof(char) * (i + 1));
    for (int j = 0; j < i; j++) {
        name[j] = env[j];
    }
    name[i] = '\0';
    return name;
}

static char *get_value(char *suite_env)
{
    char *value;
    int j = 0;

    while (suite_env[j] != '\0')
        j++;
    value = malloc(sizeof(char) * (j + 1));
    for (int i = 0; suite_env[i] != '\0'; i++) {
        value[i] = suite_env[i];
    }
    value[j] = '\0';
    return value;
}

static list_t *get_list_env(char **env)
{
    list_t *list = NULL;
    char *name;
    char *value;
    char *my_env;
    char *hostname = malloc(sizeof(char) * 256);

    gethostname(hostname, 256);
    if (env == NULL) {
        free(hostname);
        return NULL;
    }
    for (int i = 0; env[i] != NULL; i++) {
        my_env = env[i];
        name = get_name(my_env);
        value = get_value(&my_env[strlen(name) + 1]);
        add_node(&list, name, value, NULL);
    }
    add_node(&list, strdup("HOST"), hostname, NULL);
    return list;
}

static void init_local_var(shell_t *Shell)
{
    char *value = malloc(sizeof(char) * 4);

    value[0] = '0';
    value[1] = '\0';
    Shell->local = NULL;
    edit_node(&Shell->local, "?", value);
}

shell_t *init_shell(char **env)
{
    shell_t *Shell = malloc(sizeof(shell_t));

    Shell->env = get_list_env(env);
    init_local_var(Shell);
    Shell->exit_status = 0;
    Shell->status = 1;
    Shell->prev_directory = NULL;
    Shell->history = NULL;
    init_alias(Shell);
    return Shell;
}

void free_command(char **args)
{
    for (int i = 0; args[i] != NULL; i++) {
        free(args[i]);
    }
    free(args);
}

static void free_list(list_t *env)
{
    list_t *current = env;
    list_t *next;

    while (current != NULL) {
        next = current->next;
        free(current->name);
        free(current->value);
        free(current);
        current = next;
    }
}

void free_shell(shell_t *Shell)
{
    free_list(Shell->env);
    free_history(Shell->history);
    free(Shell->prev_directory);
    free(Shell);
}
