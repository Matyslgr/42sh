/*
** EPITECH PROJECT, 2024
** 42sh
** File description:
** is_alias
*/

#include "my.h"

char *browse_alias(shell_t *Shell, list_t *alias, char *name, int *i)
{
    while (alias && Shell->line) {
        if (strcmp(alias->name, name) == 0) {
            free(name);
            return strdup(alias->value);
        }
        alias = alias->next;
    }
    *i = 1;
    return name;
}

int find_alias(shell_t *Shell, list_t *alias)
{
    char *tmp = NULL;
    int i = 0;

    if (strcmp(alias->name, Shell->line) == 0) {
        tmp = strdup(alias->value);
        while (i == 0) {
            tmp = browse_alias(Shell, Shell->alias, tmp, &i);
        }
        Shell->args = split_words(tmp, " ", 0);
        free(tmp);
        return 1;
    }
    return 0;
}

int is_alias(shell_t *Shell)
{
    list_t *alias = Shell->alias;

    while (alias && Shell->line) {
        if (find_alias(Shell, alias) == 1)
            return 1;
        alias = alias->next;
    }
    return 0;
}

int count_words(const char *str)
{
    int count = 0;
    int in_word = 0;

    while (*str) {
        if (isspace(*str))
            in_word = 0;
        if (!in_word) {
            in_word = 1;
            count++;
        }
        str++;
    }
    return count;
}

static char **load_new_args(int nb_argc, int shell_argc,
    char **alias_argv, char **shell_argv)
{
    char **new_args = malloc(sizeof(char *) * (nb_argc + shell_argc));

    for (int i = 0; i < nb_argc; i++)
        new_args[i] = strdup(alias_argv[i]);
    for (int i = 1; i < shell_argc; i++)
        new_args[nb_argc + i - 1] = strdup(shell_argv[i]);
    new_args[nb_argc + shell_argc - 1] = NULL;
    free(alias_argv);
    free(shell_argv);
    return new_args;
}

int exec_alias(shell_t *Shell)
{
    list_t *alias = Shell->alias;
    int nb_argc = 0;
    int shell_argc = 0;
    char **alias_argv = NULL;
    char **shell_argv = NULL;

    while (alias != NULL) {
        if (strcmp(Shell->args[0], alias->name) == 0 && alias->oui == 0) {
            nb_argc = count_words(alias->value);
            shell_argc = count_words(Shell->line);
            alias_argv = split_words(alias->value, " ", 1);
            shell_argv = split_words(Shell->line, " ", 1);
            Shell->args = load_new_args(nb_argc, shell_argc,
                alias_argv, shell_argv);
            return 1;
        }
        alias = alias->next;
    }
    return 0;
}
