/*
** EPITECH PROJECT, 2024
** B-PSU-200-MPL-2-1-minishell2-matys.laguerre
** File description:
** my_str_to_word_array
*/

#include "my.h"

static void is_in_word(int *in_word, int *cpt)
{
    if (*in_word) {
        (*cpt)++;
        *in_word = 0;
    }
}

static int char_in_delimiters(char c, char *delimiters)
{
    for (int i = 0; delimiters[i] != '\0'; i++) {
        if (c == delimiters[i])
            return 1;
    }
    return 0;
}

static int get_nb_args(char *arg, char *delimiters)
{
    int cpt = 0;
    int in_word = 0;

    if (arg == NULL)
        return 0;
    for (int i = 0; arg[i] != '\0'; i++) {
        if (char_in_delimiters(arg[i], delimiters))
            is_in_word(&in_word, &cpt);
        else
            in_word = 1;
    }
    if (in_word)
        cpt++;
    return cpt;
}

char **my_str_to_word_array(char *arg, char *delimiters)
{
    int nb_args = get_nb_args(arg, delimiters);
    char **args = malloc(sizeof(char *) * (nb_args + 1));
    char *copy = NULL;
    char *token = NULL;
    int i = 0;

    if (nb_args == 0) {
        free(args);
        return NULL;
    }
    copy = strdup(arg);
    token = strtok(copy, delimiters);
    while (token != NULL) {
        args[i] = strdup(token);
        token = strtok(NULL, delimiters);
        i++;
    }
    args[i] = NULL;
    free(copy);
    return args;
}
