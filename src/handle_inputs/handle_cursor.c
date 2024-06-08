/*
** EPITECH PROJECT, 2024
** 42sh
** File description:
** handle_input
*/

#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include "../../includes/my.h"

void update_cursor_position_left(hi_t *v)
{
    if (v->disp_cursor_pos_x == 1) {
        fputs("\033[1A", stdout);
        for (int i = 0; i < v->term_size; i++) {
            fputs("\033[1C", stdout);
        }
        v->disp_cursor_pos_x = v->term_size;
    } else {
        fputs("\033[1D", stdout);
        v->disp_cursor_pos_x--;
    }
    v->cursor_position -= 1;
}

void update_cursor_position_right(hi_t *v)
{
    if (v->disp_cursor_pos_x == v->term_size) {
        fputs("\033[1B", stdout);
        putchar('\r');
        v->disp_cursor_pos_x = 1;
    } else {
        fputs("\033[1C", stdout);
        v->disp_cursor_pos_x++;
    }
    v->cursor_position += 1;
}
