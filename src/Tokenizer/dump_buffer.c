/*
** EPITECH PROJECT, 2024
** 42sh
** File description:
** dump_buffer
*/

#include "my.h"

static void reset_buffer(tokenize_vals_t *values)
{
    values->tokens[values->tokens_index] =
        strdup(strip_str(values->buffer));
    values->tokens_index++;
    memset(values->buffer, '\0', values->input_len + 1);
    values->buffer_index = 0;
}

static char *stringify_char(char a)
{
    char *formatted_char = malloc(sizeof(char) * 2);

    formatted_char[0] = a;
    formatted_char[1] = '\0';
    return formatted_char;
}

static void on_double_redirect(tokenize_vals_t *values, char *redirect)
{
    values->tokens[values->tokens_index] =
        strdup(strip_str(values->buffer));
    values->tokens_index++;
    memset(values->buffer, '\0', values->input_len + 1);
    values->buffer_index = 0;
    values->tokens[values->tokens_index] = strdup(redirect);
    values->tokens_index++;
    memset(values->buffer, '\0', values->input_len + 1);
    values->buffer_index = 0;
}

static bool is_space_str(char *str)
{
    if (!str)
        return true;
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] != ' ')
            return false;
    }
    return false;
}

static bool and_or_redirect(tokenize_vals_t *values, int i, const char *input)
{
    if (input[i] == '&' && input[i + 1] == '&') {
        on_double_redirect(values, "&&");
        return true;
    }
    if (input[i] == '|' && input[i + 1] == '|') {
        on_double_redirect(values, "||");
        return true;
    }
    return false;
}

bool dump_buffer(tokenize_vals_t *values, int i, const char *input)
{
    if (!values || !values->buffer)
        return false;
    if (strcmp(values->buffer, "") == 0)
        return true;
    if (!is_space_str(values->buffer)) {
        if (input[i] == '<' && input[i + 1] == '<') {
            on_double_redirect(values, "<<");
            return true;
        }
        if (input[i] == '>' && input[i + 1] == '>') {
            on_double_redirect(values, ">>");
            return true;
        }
        if (and_or_redirect(values, i, input))
            return true;
        reset_buffer(values);
    }
    values->tokens[values->tokens_index] = stringify_char(input[i]);
    values->tokens_index++;
    return true;
}
