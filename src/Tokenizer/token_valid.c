/*
** EPITECH PROJECT, 2024
** 42sh
** File description:
** token_valid
*/
#include "my.h"

static bool is_operand_present_left(const char *input, int i)
{
    i--;
    while (i >= 0 && !is_operator(input[i])) {
        if (input[i] != ' ')
            return true;
        i--;
    }
    return false;
}

static bool is_operand_present_right(const char *input, int i)
{
    i++;
    while (input[i] != '\0' && !is_operator(input[i])) {
        if (input[i] != ' ')
            return true;
        i++;
    }
    return false;
}

static bool is_pipe_valid(const char *input, int i)
{
    if (is_operand_present_left(input, i)
        && is_operand_present_right(input, i))
        return true;
    fprintf(stderr, "Invalid null command.\n");
    return false;
}

bool is_token_valid(int i, const char *input)
{
    if (input[i] == '|' && input[i + 1] != '|' && input[i - 1] != '|')
        return is_pipe_valid(input, i);
    return true;
}
