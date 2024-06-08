/*
** EPITECH PROJECT, 2024
** Shell_Programming
** File description:
** cd
*/

#include "my.h"
#include <sys/types.h>
#include <sys/stat.h>
// Cd
static void update_pwd(shell_t *Shell)
{
    list_t *current = Shell->env;
    char *current_directory = getcwd(NULL, 0);

    while (current != NULL) {
        if (strncmp("PWD", current->name, 3) == 0) {
            free(current->value);
            current->value = current_directory;
            break;
        }
        current = current->next;
    }
}

static void update_prev_directory(shell_t *Shell, char *prev_directory)
{
    free(Shell->prev_directory);
    Shell->prev_directory = prev_directory;
}

static int exist(char *path, struct stat *sb)
{
    if (S_ISDIR(sb->st_mode)) {
        return chdir(path);
    }
    write(2, path, strlen(path));
    write(2, ": Not a directory.\n", 19);
    return 1;
}

static int check_access(char *path)
{
    struct stat sb;

    if (stat(path, &sb) == 0) {
        return exist(path, &sb);
    } else {
        write(2, path, strlen(path));
        write(2, ": No such file or directory.\n", 29);
        return 1;
    }
    return 0;
}

static int get_access_directory(shell_t *Shell)
{
    if (strcmp(Shell->args[1], "-") == 0) {
        if (Shell->prev_directory == NULL) {
            write(2, ": No such file or directory.\n", 29);
            return 1;
        }
        return chdir(Shell->prev_directory);
    } else {
        return check_access(Shell->args[1]);
    }
}

static char *get_env(char *name, list_t *env)
{
    list_t *current = env;
    char *value;

    while (current != NULL) {
        if (strncmp(name, current->name, strlen(name)) == 0) {
            value = strdup(current->value);
            return value;
        }
        current = current->next;
    }
    return NULL;
}

static int cd_solo(shell_t *Shell)
{
    char *value = get_env("HOME", Shell->env);
    int res;

    if (value) {
        res = chdir(value);
        free(value);
    } else
        res = 0;
    return res;
}

int fcts_cd(shell_t *Shell)
{
    int res;
    char *prev_directory = getcwd(NULL, 0);

    if (Shell->args[1] == NULL)
        res = cd_solo(Shell);
    else
        res = get_access_directory(Shell);
    if (res == 0) {
        update_pwd(Shell);
        update_prev_directory(Shell, prev_directory);
    }
    Shell->exit_status = res;
    return 1;
}
