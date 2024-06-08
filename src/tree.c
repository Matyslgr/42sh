/*
** EPITECH PROJECT, 2024
** B-PSU-200-MPL-2-1-minishell2-matys.laguerre
** File description:
** tree
*/

#include "my.h"
#include <string.h>

static void execute_redirection(shell_t *Shell, tree_t *tree)
{
    char *str = tree->value;

    if (strncmp(str, ">>", 2) == 0) {
        execute_redirect_stdout_append(Shell, tree->left, tree->right);
        return;
    }
    if (strncmp(str, ">", 1) == 0)
        execute_redirect_stdout(Shell, tree->left, tree->right);
    if (strncmp(str, "<<", 2) == 0) {
        execute_here_doc(Shell, tree->left, tree->right);
        return;
    }
    if (strncmp(str, "<", 1) == 0)
        execute_redirect_stdin(Shell, tree->left, tree->right);
}

static void execute_operator2(shell_t *Shell, tree_t *tree)
{
    if (!strcmp(tree->value, "|")) {
        execute_pipe(Shell, tree->left, tree->right);
    } else if (!strcmp(tree->value, ";")) {
        execute_semicolon(Shell, tree->left, tree->right);
    } else {
        execute_redirection(Shell, tree);
    }
}

static void execute_operator(shell_t *Shell, tree_t *tree)
{
    if (!strcmp(tree->value, "&&")) {
        execute_and(Shell, tree->left, tree->right);
    } else if (!strcmp(tree->value, "||")) {
        execute_or(Shell, tree->left, tree->right);
    } else
        execute_operator2(Shell, tree);
}

static void exec_command(shell_t *Shell, tree_t *tree)
{
    char *command = strdup(tree->value);

    Shell->line = strdup(tree->value);
    if (is_alias(Shell) == 0)
        Shell->args = split_words(command, " \t\n\r", 1);
    if (Shell->args == NULL) {
        free(command);
        Shell->exit_status = 1;
        return;
    }
    backticks(tree, Shell);
    Shell->args = my_globbings(Shell->args);
    if (!my_vars(Shell))
        return;
    execute_command(Shell);
    free_command(Shell->args);
    free(command);
}

void execute_tree(shell_t *Shell, tree_t *tree)
{
    if (tree->type == OPERATOR) {
        execute_operator(Shell, tree);
    } else {
        exec_command(Shell, tree);
    }
}

static void free_tree(tree_t *tree)
{
    if (tree == NULL)
        return;
    free_tree(tree->left);
    free_tree(tree->right);
    free(tree->value);
    free(tree);
}

void display_on_right(
    tree_t *node, char *prefix, char new_prefix[256], int is_left)
{
    print_tree(
        node->right, strcat(strcpy(new_prefix, prefix), "    "), 0);
    printf("%s", prefix);
    printf("┌── ");
    printf("%s\n", node->value);
    print_tree(node->left,
        strcat(strcpy(new_prefix, prefix),
        (is_left ? "    " : "│   ")),
        1);
}

void print_tree(tree_t *node, char *prefix, int is_left)
{
    char new_prefix[256];

    if (node != NULL) {
        if (is_left) {
            print_tree(node->right,
                strcat(strcpy(new_prefix, prefix), "│   "), 0);
            printf("%s", prefix);
            printf("└── ");
            printf("%s\n", node->value);
            print_tree(node->left,
                strcat(strcpy(new_prefix, prefix),
                    (is_left ? "    " : "│   ")),
                1);
            return;
        }
        display_on_right(node, prefix, new_prefix, is_left);
    }
}

// Exécutez la ligne de commande
void execute_command_line(shell_t *Shell, char *command_line)
{
    char **tokenize = tokenize_ast(command_line);
    tree_t *tree;

    if (!tokenize) {
        Shell->exit_status = 1;
        return;
    }
    tree = build_tree(tokenize);
    if (Shell->disp_tree)
        print_tree(tree, "", 1);
    execute_tree(Shell, tree);
    sprintf(Shell->local->value, "%d", Shell->exit_status);
    free(command_line);
    free_tree(tree);
}
