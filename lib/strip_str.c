/*
** EPITECH PROJECT, 2024
** 42sh
** File description:
** strip_str
*/

#include "my.h"

char *strip_str(char *str)
{
    int len = strlen(str);
    int start = 0;
    int end = len - 1;
    int stripped_len;
    char *stripped_str;

    while (str[start] == ' ' || str[start] == '\t')
        start++;
    while (end >= start && (str[end] == ' ' || str[end] == '\t'))
        end--;
    stripped_len = end - start + 1;
    stripped_str = malloc(sizeof(char) * (stripped_len + 1));
    if (!stripped_str)
        return NULL;
    for (int i = 0; i < stripped_len; i++)
        stripped_str[i] = str[start + i];
    stripped_str[stripped_len] = '\0';
    return stripped_str;
}
