/*
** EPITECH PROJECT, 2024
** 42sh
** File description:
** build_tree
*/

#include "my.h"
char const *OPERATORS = "<>|&;`";

bool is_operator(char input)
{
    if (!input || !OPERATORS || input < 0)
        return 0;
    for (int i = 0; OPERATORS[i] != '\0'; i++) {
        if (input == OPERATORS[i] && input == '`')
            *backtic() = !*backtic();
        if (input == OPERATORS[i] && !*backtic())
            return true;
    }
    return false;
}

static int get_nb_separator(const char *input)
{
    int count = 0;

    for (int i = 0; input[i] != '\0'; i++) {
        if (is_operator(input[i]))
            count++;
    }
    return count;
}

static char **alloc_token_arr_ast(int nb_tokens)
{
    char **token_arr;

    if (nb_tokens == -1)
        return NULL;
    token_arr = malloc(sizeof(char *) * (nb_tokens + 1));
    if (!token_arr)
        return NULL;
    return token_arr;
}

static void tokenize_add_list_item(tokenize_vals_t *values)
{
    if (strcmp(values->buffer, "") != 0) {
        values->tokens[values->tokens_index] =
            strdup(strip_str(values->buffer));
        values->tokens_index++;
    }
    values->tokens[values->tokens_index] = NULL;
}

static int add_to_buffer(tokenize_vals_t *values, char input)
{
    if (!values || !values->buffer || values->buffer_index == -1)
        return -1;
    values->buffer[values->buffer_index] = input;
    values->buffer_index++;
    return 0;
}

static int get_nb_token(const char *input)
{
    char *input_cpy = strdup(input);
    int count = 0;
    char *token = strtok(input_cpy, OPERATORS);

    while (token != NULL) {
        count++;
        token = strtok(NULL, OPERATORS);
    }
    free(input_cpy);
    return count + get_nb_separator(input);
}

static tokenize_vals_t *init_values(const char *input)
{
    tokenize_vals_t *values = malloc(sizeof(tokenize_vals_t));

    if (!input || !values)
        return NULL;
    values->tokens = alloc_token_arr_ast(get_nb_token(input));
    values->input_len = strlen(input);
    values->buffer = malloc(sizeof(char) * values->input_len + 1);
    values->tokens_index = 0;
    values->buffer_index = 0;
    memset(values->buffer, 0, values->input_len + 1);
    return values;
}

static int operate_on_character(
    const char *input, int i, tokenize_vals_t *values)
{
    if (is_operator(input[i]) && input[i] != '`') {
        if (!is_token_valid(i, input))
            return 1;
        dump_buffer(values, i, input);
        return 2;
    }
    return 3;
}

char **tokenize_ast(const char *input)
{
    tokenize_vals_t *values = init_values(input);
    int return_val;

    if (!values)
        return NULL;
    for (int i = 0; input[i] != '\0'; i++) {
        return_val = operate_on_character(input, i, values);
        if (return_val == 2)
            continue;
        if (return_val == 1)
            return NULL;
        add_to_buffer(values, input[i]);
    }
    tokenize_add_list_item(values);
    return values->tokens;
}
