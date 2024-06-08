/*
** EPITECH PROJECT, 2024
** 42sh
** File description:
** singleton
*/

#include "my.h"

bool *background_exec(void)
{
    static bool background = 0;

    return &background;
}
