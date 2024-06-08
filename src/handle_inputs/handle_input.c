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
#include "my.h"

static int alloc_on_first_run(size_t *n, char **lineptr)
{
    if (!lineptr || *n == 0) {
        *lineptr = malloc(sizeof(char) * INPUT_BUFFER_INCREMENT);
        if (!*lineptr)
            return -1;
        memset(*lineptr, 0, (size_t) INPUT_BUFFER_INCREMENT);
        *n += INPUT_BUFFER_INCREMENT;
    }
    return 0;
}

bool is_editing_input(char user_input)
{
    if (user_input == 0x7F || user_input == 0x1B)
        return true;
    return false;
}

static void add_to_buffer(hi_t *v)
{
    if (v->user_input == '\n') {
        putchar('\n');
        return;
    }
    if (v->input_size == *v->n - 1) {
        *v->n += INPUT_BUFFER_INCREMENT;
        *v->lineptr = realloc(*v->lineptr, sizeof(char) * (*v->n));
        if (!*v->lineptr) {
            perror(SHELL_NAME);
            exit(v->shell->exit_status);
        }
        memset(
            *v->lineptr + v->input_size, 0, (size_t) INPUT_BUFFER_INCREMENT);
    }
    insert_char_at_cursor(v);
    v->input_size += 1;
}

static void handle_editing_input(hi_t *v)
{
    char key_id;

    if (v->user_input == 0x7F && v->cursor_position > 0)
        return delete_char_at_cursor(v);
    if (v->user_input == 0x1B) {
        getchar();
        key_id = getchar();
        if (key_id == 'D' && v->cursor_position > 0)
            update_cursor_position_left(v);
        if (key_id == 'C' && v->cursor_position < (int) v->input_size)
            update_cursor_position_right(v);
    }
}

static hi_t *init_hi_struct(
    char **lineptr, size_t *n, FILE *stream, shell_t *shell)
{
    hi_t *v = malloc(sizeof(hi_t));

    if (!lineptr || !n || !stream || alloc_on_first_run(n, lineptr) || !v)
        exit(shell->exit_status);
    setup_termios();
    v->disp_cursor_pos_x = get_cursor_position(shell);
    v->user_input = '\0';
    v->input_size = 0;
    v->cursor_position = 0;
    v->n = n;
    v->shell = shell;
    v->lineptr = lineptr;
    v->stream = stream;
    return v;
}

int handle_input(char **lineptr, size_t *n, FILE *stream, shell_t *shell)
{
    size_t input_size = 0;
    hi_t *v = init_hi_struct(lineptr, n, stream, shell);

    while (v->user_input != INPUT_DEFAULT_SEPARATOR) {
        v->term_size = get_terminal_width();
        v->user_input = fgetc(stdin);
        if (v->user_input == 4)
            return -1;
        if (!is_editing_input(v->user_input)) {
            add_to_buffer(v);
            v->cursor_position++;
            continue;
        }
        handle_editing_input(v);
    }
    input_size = v->input_size;
    unset_termios();
    return input_size;
}
