/*
** EPITECH PROJECT, 2024
** 42sh
** File description:
** init_alias
*/

#include "my.h"

void add_node_if_bin(list_t **list, char *name, char *value, char *alias)
{
    list_t *node = malloc(sizeof(list_t));
    list_t *tmp = *list;

    node->name = name;
    node->value = value;
    node->full_name = alias;
    node->oui = 1;
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

void init_alias(shell_t *Shell)
{
    Shell->alias = NULL;
    add_node_if_bin(&Shell->alias, "ls", "ls --color=auto", "ls");
    add_node(&Shell->alias, "ll", "ls -l", "ll");
    add_node(&Shell->alias, "la", "ls -a", "la");
    add_node(&Shell->alias, "l", "ls -l", "l");
}
