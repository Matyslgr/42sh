/*
** EPITECH PROJECT, 2024
** 42sh
** File description:
** build_tree
*/

#include "my.h"

const precedence_asso_t PRECEDENCE_MAP[] = {
    {1, ";"}, {2, "&&"}, {2, "||"}, {3, "|"},
    {4, "<"}, {4, ">"}, {4, ">>"}, {4, "<<"}, {-1, "XX"}};

int get_precedence(char *op_str)
{
    for (int i = 0; PRECEDENCE_MAP[i].precedence != -1; i++) {
        if (strcmp(PRECEDENCE_MAP[i].op_str, op_str) == 0)
            return PRECEDENCE_MAP[i].precedence;
    }
    return -1;
}

tree_t *new_node(char *value, ast_node_type_t type)
{
    tree_t *node = malloc(sizeof(tree_t));

    if (!node)
        return NULL;
    node->value = value;
    node->type = type;
    node->left = NULL;
    node->right = NULL;
    return node;
}

static void process_token(ast_val_t *values, int i, char **tokens)
{
    tree_t *operator;
    tree_t *left;
    tree_t *right;

    values->token = tokens[i];
    if (get_precedence(values->token) != -1) {
        while (values->stack_size > 0
            && get_precedence(values->token) <= get_precedence(
                values->stack[values->stack_size - 1]->value)) {
            operator= values->stack[DEC_STACK_SIZE];
            right = values->tree[DEC_TREE_SIZE];
            left = values->tree[DEC_TREE_SIZE];
            operator->right = right;
            operator->left = left;
            values->tree[INC_TREE_SIZE] = operator;
        }
        values->stack[INC_STACK_SIZE] = new_node(values->token, OPERATOR);
        return;
    }
    values->tree[INC_TREE_SIZE] = new_node(values->token, OPERAND);
}

static void init_values(ast_val_t *values, int nb_tokens, char **tokens)
{
    values->stack = malloc(sizeof(tree_t *) * nb_tokens);
    values->tree = malloc(sizeof(tree_t *) * nb_tokens);
    values->stack_size = 0;
    values->tree_size = 0;
    for (int i = 0; tokens[i] != NULL; i++)
        process_token(values, i, tokens);
}

static void free_values(ast_val_t *values)
{
    free(values->stack);
    free(values->tree);
    free(values);
}

int get_token_arr_size(char **tokens)
{
    int nb_tokens = 0;

    if (!tokens)
        return -1;
    while (tokens[nb_tokens])
        nb_tokens++;
    return nb_tokens;
}

tree_t *build_tree(char **tokens)
{
    tree_t *root = NULL;
    tree_t *operator = NULL;
    tree_t *left = NULL;
    tree_t *right = NULL;
    ast_val_t *values = malloc(sizeof(ast_val_t));
    int nb_tokens = get_token_arr_size(tokens);

    init_values(values, nb_tokens, tokens);
    while (values->stack_size > 0) {
        operator= values->stack[DEC_STACK_SIZE];
        right = values->tree[DEC_TREE_SIZE];
        left = values->tree[DEC_TREE_SIZE];
        operator->right = right;
        operator->left = left;
        values->tree[INC_TREE_SIZE] = operator;
    }
    root = values->tree[0];
    free_values(values);
    *backtic() = false;
    return root;
}
