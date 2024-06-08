/*
** EPITECH PROJECT, 2024
** 42sh
** File description:
** manage_terminal.c
*/

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include "../../includes/my.h"

static void display_and_check_newline_input(hi_t *v, char *to_display)
{
    for (size_t i = 0; i < strlen(to_display); i++) {
        if (v->disp_cursor_pos_x == v->term_size + 1) {
            putchar('\n');
            v->disp_cursor_pos_x = 1;
        }
        putchar(to_display[i]);
        v->disp_cursor_pos_x++;
    }
}

void display_updated_line_part_input(
    hi_t *v, int cursor_position, bool recursive)
{
    char *to_display = &(*v->lineptr)[cursor_position];
    int save = v->disp_cursor_pos_x;

    fputs("\0337", stdout);
    if (v->disp_cursor_pos_x == v->term_size) {
        putchar(to_display[0]);
        putchar('\n');
        v->disp_cursor_pos_x = 1;
        display_updated_line_part_input(v, cursor_position + 1, true);
        return;
    }
    display_and_check_newline_input(v, to_display);
    fputs("\0338", stdout);
    v->disp_cursor_pos_x = save;
    if (!recursive) {
        fputs("\033[1C", stdout);
        v->disp_cursor_pos_x++;
    }
}

static void delete_chars_from_cursor(hi_t *v)
{
    if (v->disp_cursor_pos_x == 1) {
        fputs("\033[1A", stdout);
        for (int i = 0; i < v->term_size + 1; i++) {
            fputs("\033[1C", stdout);
        }
        v->disp_cursor_pos_x = v->term_size;
    } else {
        fputs("\033[1D", stdout);
        v->disp_cursor_pos_x--;
    }
    fputs("\033[0J", stdout);
    fputs("\0337", stdout);
}

void display_updated_line_part_delete(hi_t *v)
{
    char *to_display = &(*v->lineptr)[v->cursor_position];
    int save;

    delete_chars_from_cursor(v);
    save = v->disp_cursor_pos_x;
    for (size_t i = 0; i < strlen(to_display); i++) {
        if (v->disp_cursor_pos_x == v->term_size + 1) {
            putchar('\n');
            v->disp_cursor_pos_x = 1;
        }
        putchar(to_display[i]);
        v->disp_cursor_pos_x++;
    }
    v->disp_cursor_pos_x = save;
    fputs("\0338", stdout);
}

void insert_char_at_cursor(hi_t *v)
{
    int i = v->input_size;

    for (; i >= v->cursor_position + 1; i--) {
        (*v->lineptr)[i] = (*v->lineptr)[i - 1];
    }
    (*v->lineptr)[i] = v->user_input;
    display_updated_line_part_input(v, v->cursor_position, false);
}

void delete_char_at_cursor(hi_t *v)
{
    int i = v->cursor_position - 1;

    for (; i <= (int) v->input_size; i++) {
        (*v->lineptr)[i] = (*v->lineptr)[i + 1];
    }
    v->cursor_position -= 1;
    v->input_size -= 1;
    display_updated_line_part_delete(v);
}

void display_prompt(shell_t *Shell)
{
    char *host = strdup(get_env_value(Shell->env, "HOST"));
    char *pwd = strdup(get_env_value(Shell->env, "PWD"));
    char *home = strdup(get_env_value(Shell->env, "HOME"));
    char *new_pwd = NULL;
    char *tmp = NULL;

    if (strncmp(pwd, home, strlen(home)) == 0) {
        new_pwd = pwd + strlen(home);
        tmp = strcatdup("~", new_pwd);
    } else
        tmp = strdup(pwd);
    printf("\033[4;31m%s", host);
    printf("\033[0m");
    printf("\033[1;37m:%s> ", tmp);
    printf("\033[0m");
    free(host);
    free(pwd);
    free(home);
    free(tmp);
}
