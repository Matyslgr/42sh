/*
** EPITECH PROJECT, 2024
** 42sh
** File description:
** strcatdup
*/

#include "my.h"

char *strcatdup(char *str1, char *str2)
{
    char *str = malloc(sizeof(char) * (strlen(str1) + strlen(str2) + 1));

    str[0] = '\0';
    strcat(str, str1);
    strcat(str, str2);
    return str;
}
