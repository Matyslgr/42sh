/*
** EPITECH PROJECT, 2024
** 42sh
** File description:
** unmatch_quotes
*/

#include "my.h"

static int check_double_quotes(char c, bool *double_quotes, bool single_quotes,
    bool backticks)
{
    if (single_quotes || backticks)
        return 1;
    if (c == '"') {
        *double_quotes = !*double_quotes;
    }
    return 1;
}

static int check_single_quotes(char c, bool *single_quotes, bool double_quotes,
    bool backticks)
{
    if (double_quotes || backticks)
        return 1;
    if (c == '\'') {
        *single_quotes = !*single_quotes;
    }
    return 1;
}

static int check_backticks(char c, bool *backticks, bool double_quotes,
    bool single_quotes)
{
    if (double_quotes || single_quotes)
        return 1;
    if (c == '`') {
        *backticks = !*backticks;
    }
    return 1;
}

static int check_all_quotes(bool doble, bool single, bool backticks)
{
    if (doble) {
        fprintf(stderr, "Unmatched '\"'.\n");
        return 1;
    }
    if (single) {
        fprintf(stderr, "Unmatched '''.\n");
        return 1;
    }
    if (backticks) {
        fprintf(stderr, "Unmatched '`'.\n");
        return 1;
    }
    return 0;
}

int unmatched_quotes(char *str)
{
    bool doble = false;
    bool single = false;
    bool backticks = false;

    for (int i = 0; str[i] != '\0'; i++) {
        if ((i == 0 || str[i - 1] != '\\') && !check_double_quotes
            (str[i], &doble, single, backticks))
            return 1;
        if ((i == 0 || str[i - 1] != '\\') && !check_single_quotes
            (str[i], &single, doble, backticks))
            return 1;
        if ((i == 0 || str[i - 1] != '\\') && !check_backticks
            (str[i], &backticks, doble, single))
            return 1;
    }
    return check_all_quotes(doble, single, backticks);
}
