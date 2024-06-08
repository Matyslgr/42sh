/*
** EPITECH PROJECT, 2024
** 42sh
** File description:
** history
*/

#include "my.h"

char *get_time(void)
{
    time_t rawtime;
    struct tm *timeinfo;
    char *buffer = malloc(20 * sizeof(char));

    memset(buffer, 0, 20);
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(buffer, 20, TIME_FORMAT, timeinfo);
    return buffer;
}

static char *get_last_command_used(shell_t *Shell, char *command)
{
    history_t *tmp = Shell->history;

    while (tmp && tmp->next)
        tmp = tmp->next;
    while (tmp) {
        if (!strncmp(command, tmp->command, strlen(command))) {
            return strdup(tmp->command);
        }
        tmp = tmp->prev;
    }
    return NULL;
}

static char *get_last_command(shell_t *Shell)
{
    history_t *tmp = Shell->history;

    while (tmp && tmp->next)
        tmp = tmp->next;
    if (tmp)
        return strdup(tmp->command);
    return NULL;
}

static char *event_designator(shell_t *Shell, char *command)
{
    char *new_command = NULL;

    if (command[0] == '!' && command[1] == '!') {
        if (Shell->history) {
            free(command);
            command = get_last_command(Shell);
        }
        return command;
    }
    if (command[0] == '!' && command[1] != '\0') {
        new_command = get_last_command_used(Shell, &command[1]);
        if (new_command) {
            free(command);
            printf("%s\n", new_command);
            command = new_command;
        }
    }
    return command;
}

void save_command(shell_t *Shell, char **command)
{
    history_t *new = malloc(sizeof(history_t));
    history_t *tmp = Shell->history;
    static int index = 1;

    *command = event_designator(Shell, *command);
    new->index = index;
    index++;
    new->time = get_time();
    new->command = strdup(*command);
    new->next = NULL;
    new->prev = NULL;
    while (tmp && tmp->next)
        tmp = tmp->next;
    if (tmp)
        tmp->next = new;
    else
        Shell->history = new;
    new->prev = tmp;
}
