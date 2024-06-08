/*
** EPITECH PROJECT, 2024
** 42sh
** File description:
** setup_termios
*/

#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include "my.h"

void unset_termios(void)
{
    struct termios term;

    if (isatty(STDIN_FILENO)) {
        tcgetattr(STDIN_FILENO, &term);
        term.c_lflag |= (ECHO | ICANON);
        tcsetattr(STDIN_FILENO, TCSAFLUSH, &term);
    }
}

void setup_termios(void)
{
    struct termios pty;

    if (isatty(STDIN_FILENO)) {
        tcgetattr(STDIN_FILENO, &pty);
        pty.c_lflag &= ~(ECHO | ICANON);
        tcsetattr(STDIN_FILENO, TCSAFLUSH, &pty);
    }
}
