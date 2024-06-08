/*
** EPITECH PROJECT, 2024
** 42sh
** File description:
** set
*/

#include "my.h"

static int get_nb_alphanum(char *str)
{
    int i = 0;

    if (str[i] == '?')
        return 1;
    while (str[i] != '\0' && ((str[i] >= 'a' && str[i] <= 'z') ||
    (str[i] >= 'A' && str[i] <= 'Z') || (str[i] >= '0' && str[i] <= '9')))
        i++;
    return i;
}

static char *get_local_value(shell_t *Shell, char *name)
{
    list_t *tmp = Shell->local;
    int nb_chars = 0;

    name++;
    nb_chars = get_nb_alphanum(name);
    while (tmp != NULL) {
        if (tmp->name != NULL && strncmp(tmp->name, name, nb_chars) == 0) {
            return strdup(tmp->value);
        }
        tmp = tmp->next;
    }
    return NULL;
}

int edit(list_t **list, list_t **tmp, char *name, char *value)
{
    if (strcmp((*list)->name, name) == 0) {
        (*list)->value = value;
        return 1;
    }
    while ((*tmp)->next != NULL) {
        if (strcmp((*tmp)->name, name) == 0) {
            (*tmp)->value = value;
            return 1;
            }
        (*tmp) = (*tmp)->next;
    }
    if (strcmp((*tmp)->name, name) == 0) {
        (*tmp)->value = value;
        return 1;
    }
    return 0;
}

void edit_node(list_t **list, char *name, char *value)
{
    list_t *node = malloc(sizeof(list_t));
    list_t *tmp = *list;

    node->name = name;
    node->value = value;
    node->next = NULL;
    if (*list == NULL) {
        *list = node;
        return;
    }
    if (edit(list, &tmp, name, value) == 1) {
        free(node);
        return;
    }
    tmp->next = node;
}

// need to fix exit status
int fcts_set(shell_t *Shell)
{
    char **variable = NULL;

    if (my_tablen(Shell->args) == 2) {
        variable = my_str_to_word_array(Shell->args[1], "=");
        if (my_tablen(variable) != 2 || variable[0][0] < 'A' ||
        (variable[0][0] > 'Z' && variable[0][0] < 'a') ||
        variable[0][0] > 'z') {
            write(2,
            "set: Variable name must begin with a letter.\n", 45);
            Shell->exit_status = 1;
            return 0;
        }
        edit_node(&Shell->local, variable[0], variable[1]);
        free(variable);
    } else {
        write(2, "set: Wrong number of arguments.\n", 32);
        Shell->exit_status = 1;
    }
    Shell->exit_status = 0;
    return 0;
}

int fcts_unset(shell_t *Shell)
{
    char *name = NULL;
    list_t *tmp = Shell->local;

    if (my_tablen(Shell->args) != 2) {
        write(2, "unset: Wrong number of arguments.\n", 34);
        Shell->exit_status = 1;
        return 0;
    }
    name = Shell->args[1];
    while (tmp != NULL) {
        if (strcmp(tmp->name, name) == 0) {
            tmp->name = NULL;
            tmp->value = NULL;
            Shell->exit_status = 0;
            return 0;
        }
        tmp = tmp->next;
    }
    Shell->exit_status = 0;
    return 0;
}

static void error_var(shell_t *Shell, int i, int j)
{
    char *tmp = strndup(Shell->args[i] + j + 1,
    get_nb_alphanum(Shell->args[i] + j + 1));

    dprintf(2, "%s: Undefined variable.\n", tmp);
    free(tmp);
    Shell->args[i] = NULL;
}

static void get_new_args(shell_t *Shell, int i, int j, char *var)
{
    char *pre = NULL;
    char *post = NULL;
    int post_len = strlen(Shell->args[i] +
    j + get_nb_alphanum(Shell->args[i] + j + 1) + 1);

    pre = malloc(j + 1);
    memcpy(pre, Shell->args[i], j);
    pre[j] = '\0';
    post = malloc(post_len + 1);
    memcpy(post, Shell->args[i] +
    j + get_nb_alphanum(Shell->args[i] + j + 1) + 1, post_len);
    post[post_len] = '\0';
    Shell->args[i] = malloc(j + strlen(var) + post_len + 1);
    sprintf(Shell->args[i], "%s%s%s", pre, var, post);
    free(pre);
    free(post);
}

static void update_local_var(shell_t *Shell, int i, int j)
{
    char *var = NULL;

    if (Shell->args[i][j] == '$') {
        var = get_local_value(Shell, Shell->args[i] + j);
        if (var == NULL) {
            error_var(Shell, i, j);
            return;
        }
        get_new_args(Shell, i, j, var);
    }
}

int my_vars(shell_t *Shell)
{
    for (int i = 0; Shell->args[i] != NULL; i++) {
        for (int j = 0; Shell->args[i] != NULL &&
            Shell->args[i][j] != '\0'; j++) {
            update_local_var(Shell, i, j);
        }
        if (Shell->args[i] == NULL) {
            Shell->exit_status = 1;
            return 0;
        }
    }
    return 1;
}
