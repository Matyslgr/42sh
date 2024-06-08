/*
** EPITECH PROJECT, 2024
** B-PSU-200-MPL-2-1-minishell2-alexyan.comino
** File description:
** globbings
*/

#include "my.h"
#include <glob.h>

char **remove_hidden_files(char **input)
{
    int nb_files = 0;
    char **new_inp = NULL;
    int k = 0;

    for (int i = 0; input[i] != NULL; i++) {
        if (input[i][0] != '.')
            nb_files++;
    }
    new_inp = malloc(sizeof(char *) * (nb_files + 1));
    new_inp[nb_files] = NULL;
    for (int i = 0; input[k] != NULL; k++) {
        if (input[k][0] != '.') {
            new_inp[i] = strdup(input[k]);
            i++;
        }
    }
    free(input);
    return new_inp;
}

char **get_globbings(char *arg)
{
    glob_t globbing;
    char **new_inp = NULL;

    if (arg == NULL)
        return NULL;
    else if (strcmp(arg, "..") == 0) {
        new_inp = my_str_to_word_array(arg, " ");
        return new_inp;
    }
    if (glob(arg, GLOB_PERIOD | GLOB_NOSORT | GLOB_NOCHECK | GLOB_NOMAGIC,
    NULL, &globbing) == GLOB_NOMATCH)
        return NULL;
    new_inp = my_tabdup(globbing.gl_pathv);
    new_inp = remove_hidden_files(new_inp);
    globfree(&globbing);
    return new_inp;
}

char **my_globbings(char **input)
{
    char **new_inp = NULL;
    char **temp_tab = NULL;

    if (input == NULL || my_tablen(input) == 1)
        return input;
    for (int i = 0; input[i] != NULL; i++) {
        temp_tab = get_globbings(input[i]);
        new_inp = my_tabcat(new_inp, temp_tab);
    }
    for (int i = 0; input[i] != NULL; i++)
        free(input[i]);
    free(input);
    return new_inp;
}
