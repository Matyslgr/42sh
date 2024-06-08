/*
** EPITECH PROJECT, 2024
** 42sh
** File description:
** get_terminal_infos.c
*/

#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include "../../includes/my.h"

int get_terminal_width(void)
{
    struct winsize ws;

    ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws);
    return ws.ws_col;
}

int get_cursor_position(shell_t *shell)
{
    int y = -1;
    int x = -1;

    fflush(stdout);
    fputs("\033[6n", stdout);
    scanf("\033[%d;%dR", &y, &x);
    if (y < 0 || x < 0) {
        fprintf(stderr, "Incompatible terminal.\n");
        exit(shell->exit_status);
    }
    return x;
}
