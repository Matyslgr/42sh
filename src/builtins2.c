/*
** EPITECH PROJECT, 2024
** 42sh
** File description:
** builtins2
*/

#include "my.h"

static int get_max_index(history_t *history)
{
    int max = 0;

    while (history) {
        if (history->index > max)
            max = history->index;
        history = history->next;
    }
    return max;
}

static void normal_or_simple(history_t *tmp, int simple)
{
    if (simple) {
        printf("%s\n", tmp->command);
    } else {
        printf("\t%d %s %s\n", tmp->index, tmp->time, tmp->command);
    }
}

static void display_history_normal(history_t *tmp, int num, int simple)
{
    int max = get_max_index(tmp);

    if (num == -1)
        num = max;
    while (tmp && tmp->index < max - num + 1)
        tmp = tmp->next;
    while (tmp) {
        normal_or_simple(tmp, simple);
        tmp = tmp->next;
    }
}

static void display_history_reverse(history_t *tmp, int num, int simple)
{
    int i = 0;

    if (num == -1)
        num = get_max_index(tmp);
    while (tmp->next)
        tmp = tmp->next;
    while (tmp && i < num) {
        normal_or_simple(tmp, simple);
        tmp = tmp->prev;
        i++;
    }
}

static void display_history(shell_t *Shell, history_flags_t *flags)
{
    history_t *tmp = Shell->history;

    if (flags->r)
        display_history_reverse(tmp, flags->num, flags->h);
    else
        display_history_normal(tmp, flags->num, flags->h);
}

static void init_history_flags(history_flags_t *flags)
{
    flags->c = false;
    flags->r = false;
    flags->h = false;
    flags->num = -1;
}

static void check_flags(char *flag, history_flags_t *flags)
{
    for (int i = 1; flag[i] != '\0'; i++) {
        switch (flag[i]) {
        case 'c':
            flags->c = true;
            break;
        case 'r':
            flags->r = true;
            break;
        case 'h':
            flags->h = true;
            break;
        default:
            fprintf(stderr, "history: Invalid option\n");
            break;
        }
    }
}

static void parse_history_flags(char **argv, history_flags_t *flags)
{
    for (int i = 1; argv[i] != NULL; i++) {
        if (argv[i][0] >= '0' && argv[i][0] <= '9') {
            flags->num = my_getnbr(argv[i]);
        }
        if (argv[i][0] == '-')
            check_flags(argv[i], flags);
    }
}

void free_history(history_t *history)
{
    history_t *tmp = history;
    history_t *next = NULL;

    while (tmp) {
        next = tmp->next;
        free(tmp->command);
        free(tmp->time);
        free(tmp);
        tmp = next;
    }
}

int fcts_history(shell_t *Shell)
{
    history_flags_t *flags = malloc(sizeof(history_flags_t));

    init_history_flags(flags);
    parse_history_flags(Shell->args, flags);
    if (flags->c) {
        free_history(Shell->history);
        free(flags);
        Shell->history = NULL;
        Shell->exit_status = 0;
        return 0;
    }
    display_history(Shell, flags);
    Shell->exit_status = 0;
    free(flags);
    return 0;
}
