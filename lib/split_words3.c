/*
** EPITECH PROJECT, 2024
** 42sh
** File description:
** split_words3
*/

#include "my.h"

static void remove_char(char *str, int index)
{
    for (int i = index; str[i] != '\0'; i++)
        str[i] = str[i + 1];
}

static void replace_char(char *str, int index, char c)
{
    if (str[index] == '\0')
        return;
    str[index] = c;
    remove_char(str, index + 1);
}

void update_str(char *str, int i)
{
    if (str[i] == ' ')
        remove_char(str, i - 1);
    if (str[i] == '\\' && str[i + 1] == 't') {
        remove_char(str, i - 1);
        replace_char(str, i - 1, '\t');
    }
    if (str[i] == '\\' && str[i + 1] == 'n') {
        remove_char(str, i - 1);
        replace_char(str, i - 1, '\n');
    }
}
