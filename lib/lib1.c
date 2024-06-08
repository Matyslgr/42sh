/*
** EPITECH PROJECT, 2024
** B-PSU-200-MPL-2-1-minishell2-matys.laguerre
** File description:
** fcts_lib3
*/

#include "my.h"

int my_puterror(char *str)
{
    write(2, str, strlen(str));
    return 0;
}

int my_tablen(char **tab)
{
    int cpt = 0;

    if (tab == NULL)
        return 0;
    for (int i = 0; tab[i] != NULL; i++)
        cpt++;
    return cpt;
}

char **my_tabdup(char **tab)
{
    char **new_tab = NULL;

    if (tab == NULL)
        return NULL;
    new_tab = malloc(sizeof(char *) * (my_tablen(tab) + 1));
    for (int i = 0; tab[i] != NULL; ++i)
        new_tab[i] = strdup(tab[i]);
    new_tab[my_tablen(tab)] = NULL;
    return new_tab;
}

char **my_tabcat(char **tab1, char **tab2)
{
    int len1 = my_tablen(tab1);
    int len2 = my_tablen(tab2);
    char **new_tab = malloc(sizeof(char *) * (len1 + len2 + 1));
    int idx = 0;

    for (int i = 0; tab1 != NULL && tab1[i] != NULL; ++i) {
        new_tab[idx] = strdup(tab1[i]);
        idx++;
    }
    if (tab1 != NULL)
        free(tab1);
    for (int i = 0; tab2 != NULL && tab2[i] != NULL; ++i) {
        new_tab[idx] = strdup(tab2[i]);
        idx++;
    }
    if (tab2 != NULL)
        free(tab2);
    new_tab[len1 + len2] = NULL;
    return new_tab;
}
