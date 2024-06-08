/*
** EPITECH PROJECT, 2024
** 42sh
** File description:
** operators_and_or
*/

#include "my.h"

void execute_and(shell_t *Shell, tree_t *left, tree_t *right)
{
    execute_tree(Shell, left);
    if (Shell->exit_status == 0)
        execute_tree(Shell, right);
}

void execute_or(shell_t *Shell, tree_t *left, tree_t *right)
{
    execute_tree(Shell, left);
    if (Shell->exit_status != 0)
        execute_tree(Shell, right);
}
