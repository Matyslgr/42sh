/*
** EPITECH PROJECT, 2024
** 42sh
** File description:
** alias
*/

#include "my.h"

char *change_arg(shell_t *Shell, char *tmp)
{
    tmp = strdup(Shell->args[1]);
    if (my_tablen(Shell->args) > 2) {
        for (int i = 2; i < my_tablen(Shell->args); i++) {
            tmp = strcatdup(tmp, " ");
            tmp = strcatdup(tmp, Shell->args[i]);
        }
    }
    return tmp;
}

static void is_two_argv(char **arr, list_t *alias)
{
    if (my_tablen(arr) > 1)
        printf("(%s)\n", alias->value);
    else
        printf("%s\n", alias->value);
}

void only_alias(shell_t *Shell, list_t *alias)
{
    char **arr = NULL;

    if (my_tablen(Shell->args) == 1) {
        while (alias) {
            arr = split_words(alias->value, " ", 0);
            printf("%s\t", alias->name);
            is_two_argv(arr, alias);
            alias = alias->next;
        }
    }
    Shell->exit_status = 0;
}

static char *new_str(char **arr, char *str)
{
    str = strdup(arr[1]);
    for (int i = 2; arr[i]; i++) {
        str = strcatdup(str, " ");
        str = strcatdup(str, arr[i]);
    }
    return str;
}

static void check_alias(char **arr, list_t *alias)
{
    if (strcmp(arr[0], arr[1]) == 0)
        alias->oui = 1;
}

int fcts_alias(shell_t *Shell)
{
    list_t *alias = Shell->alias;
    char **arr = NULL;
    char *tmp = NULL;
    char *str = NULL;

    if (my_tablen(Shell->args) != 1) {
        tmp = change_arg(Shell, tmp);
        arr = split_words(tmp, " ", 1);
        if (my_tablen(arr) <= 1)
            return 1;
        str = new_str(arr, str);
        delete_element(&alias, arr[0]);
        add_node(&alias, arr[0], str, Shell->line);
        check_alias(arr, alias);
        Shell->alias = alias;
        return 1;
    }
    only_alias(Shell, alias);
    Shell->exit_status = 0;
    return 1;
}

int fcts_unalias(shell_t *Shell)
{
    list_t *alias = Shell->alias;
    char **arr = NULL;

    if (my_tablen(Shell->args) != 1) {
        arr = split_words(Shell->args[1], " ", 1);
        delete_element(&alias, arr[0]);
        Shell->alias = alias;
        Shell->exit_status = 0;
        return 1;
    }
    return 1;
}
