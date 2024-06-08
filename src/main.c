/*
** EPITECH PROJECT, 2024
** B-PSU-200-MPL-2-1-minishell1-matys.laguerre
** File description:
** main
*/

#include "my.h"

int main(int argc, char **, char **env)
{
    char *command_line;
    shell_t *Shell = init_shell(env);
    int exit_status = 0;

    Shell->disp_tree = false;
    if (argc != 1)
        return 1;
    while (Shell->status) {
        command_line = get_command(Shell);
        if (!Shell->status) {
            free(command_line);
            break;
        }
        save_command(Shell, &command_line);
        execute_command_line(Shell, command_line);
    }
    exit_status = Shell->exit_status;
    free_shell(Shell);
    return exit_status;
}
