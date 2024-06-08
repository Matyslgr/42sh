/*
** EPITECH PROJECT, 2024
** 42sh
** File description:
** list
*/

#include "my.h"

void del(list_t **Shell, const char *name, list_t *current, list_t *previous)
{
    if (strcmp(current->name, name) == 0) {
        if (previous == NULL)
            *Shell = current->next;
        else
            previous->next = current->next;
        return;
    }
}

void delete_element(list_t **Shell, const char *name)
{
    list_t *current = *Shell;
    list_t *previous = NULL;

    if (*Shell == NULL)
        return;
    while (current != NULL) {
        del(Shell, name, current, previous);
        previous = current;
        current = current->next;
    }
}
