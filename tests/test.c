/*
** EPITECH PROJECT, 2024
** B-PSU-200-MPL-2-1-minishell1-matys.laguerre
** File description:
** test
*/

// tests_minishell.c
#include <criterion/criterion.h>
#include <criterion/redirect.h>
#include "my.h"  // Inclure le fichier contenant vos fonctions

void redirect_all_std(void)
{
    cr_redirect_stdout();
    cr_redirect_stderr();
}

// minishell.c
Test(init_shell, env_no_equal)
{
    char **env = malloc(sizeof(char *) * 3);
    env[0] = strdup("PATH");
    env[1] = strdup("USER");
    env[2] = strdup("PWD");
    env[3] = NULL;
    shell_t *result = init_shell(env);
    cr_assert_not_null(result);
    free(env);
}
// add node
Test(add_node, should_return_list)
{
    list_t *list = NULL;
    add_node(&list, "USER", "matys", NULL);
    cr_assert_not_null(list);
}

Test(add_node, should_return_list2)
{
    list_t *list = NULL;
    add_node(&list, "USER", NULL, NULL);
    cr_assert_not_null(list);
}

Test(free_command, should_return_null_command)
{
    char **args = malloc(sizeof(char *) * 3);
    args[0] = strdup("ls");
    args[1] = strdup("-l");
    args[2] = NULL;
    free_command(args);
    cr_assert_not_null(args);
}

Test(free_shell, should_return_null_shell)
{
    char *env[] = {"PATH=/usr/bin", "USER=matys", NULL};
    shell_t *shell = init_shell(env);
    free_shell(shell);
    cr_assert_not_null(shell);
}

// builtins.c
Test(fcts_cd, should_return_1)
{
    char *env[] = {"PATH=/usr/bin", "USER=matys", "PWD=/home/matys", NULL};
    shell_t *shell = init_shell(env);
    shell->args = malloc(sizeof(char *) * 3);
    shell->args[0] = strdup("cd");
    shell->args[1] = strdup("..");
    shell->args[2] = NULL;
    int result = fcts_cd(shell);
    cr_assert_eq(result, 1);
}

Test(fcts_cd, cd_null)
{
    char *env[] = {"PATH=/usr/bin", "USER=matys", "PWD=/home/matys", NULL};
    shell_t *shell = init_shell(env);
    shell->prev_directory = NULL;
    shell->args = malloc(sizeof(char *) * 3);
    shell->args[0] = strdup("cd");
    shell->args[1] = strdup("-");
    shell->args[2] = NULL;
    int returned = fcts_cd(shell);
    cr_assert_neq(shell->exit_status, 0);
}

Test(fcts_cd, cd_)
{
    char *env[] = {"PATH=/usr/bin", "USER=matys", "PWD=/home/matys", NULL};
    shell_t *shell = init_shell(env);
    shell->args = malloc(sizeof(char *) * 3);
    shell->args[0] = strdup("cd");
    shell->args[1] = strdup("..");
    shell->args[2] = NULL;
    int result = fcts_cd(shell);
    cr_assert_eq(result, 1);
    free(shell->args[0]);
    shell->args[0] = strdup("cd");
    shell->args[1] = strdup("-");
    shell->args[2] = NULL;
    fcts_cd(shell);
    cr_assert_eq(shell->exit_status, 0);
}

Test(fcts_cd, should_return_84)
{
    char *env[] = {"PATH=/usr/bin", "USER=matys", "PWD=/home/matys", "HOME=/home/matys", NULL};
    shell_t *shell = init_shell(env);
    shell->args = malloc(sizeof(char *) * 2);
    shell->args[0] = strdup("cd");
    shell->args[1] = NULL;
    int result = fcts_cd(shell);
    cr_assert_not_null(shell);
}

Test(fcts_cd, no_home)
{
    char *env[] = {"PATH=/usr/bin", "USER=matys", "PWD=/home/matys", NULL};
    shell_t *shell = init_shell(env);
    shell->args = malloc(sizeof(char *) * 2);
    shell->args[0] = strdup("cd");
    shell->args[1] = NULL;
    int result = fcts_cd(shell);
    cr_assert_not_null(shell);
}

Test(fcts_cd, env_null)
{
    char **env = NULL;
    shell_t *shell = init_shell(env);
    shell->args = malloc(sizeof(char *) * 3);
    shell->args[0] = strdup("cd");
    shell->args[1] = strdup("..");
    shell->args[2] = NULL;
    int result = fcts_cd(shell);
    cr_assert_eq(result, 1);
}

Test(fcts_cd, cd_home)
{
    char *env[] = {"PATH=/usr/bin", "USER=matys", "HOME=/home/matys", NULL};
    shell_t *shell = init_shell(env);
    shell->args = malloc(sizeof(char *) * 2);
    shell->args[0] = strdup("cd");
    shell->args[1] = NULL;
    int result = fcts_cd(shell);
    cr_assert_eq(result, 1);
}

Test(fcts_cd, cd_file)
{
    char *env[] = {"PATH=/usr/bin", "USER=matys", "PWD=/home/matys", NULL};
    shell_t *shell = init_shell(env);
    shell->args = malloc(sizeof(char *) * 3);
    shell->args[0] = strdup("cd");
    shell->args[1] = strdup("/bin/ls");
    shell->args[2] = NULL;
    int result = fcts_cd(shell);
    cr_assert_eq(result, 1);
}

Test(fcts_cd, cd_not_exist)
{
    char *env[] = {"PATH=/usr/bin", "USER=matys", "PWD=/home/matys", NULL};
    shell_t *shell = init_shell(env);
    shell->args = malloc(sizeof(char *) * 3);
    shell->args[0] = strdup("cd");
    shell->args[1] = strdup("ncerverv");
    shell->args[2] = NULL;
    int result = fcts_cd(shell);
    cr_assert_eq(result, 1);
}

// get env value
Test(get_env_value, should_return_null)
{
    list_t *list = malloc(sizeof(list_t));
    list->name = strdup("USER");
    list->value = strdup("bobo");
    list->next = NULL;
    char *result = get_env_value(list, "rcrec");
    cr_assert_null(result);
}

Test(get_env_value, should_return_bobo)
{
    list_t *list = malloc(sizeof(list_t));
    list->name = strdup("USER");
    list->value = strdup("bobo");
    list->next = NULL;
    char *result = get_env_value(list, "USER");
    cr_assert_str_eq(result, "bobo");
}
// unsetenv
Test(fcts_unsetenv, should_return_1)
{
    char *env[] = {"PATH=/usr/bin", "USER=matys", "PWD=/home/matys", NULL};
    shell_t *shell = init_shell(env);
    int result;
    shell->args = malloc(sizeof(char *) * 3);
    shell->args[0] = strdup("unsetenv");
    shell->args[1] = strdup("USER");
    shell->args[2] = NULL;
    result = fcts_unsetenv(shell);
    cr_assert_eq(result, 1);
}

Test(fcts_unsetenv, should_return_error)
{
    char *env[] = {"PATH=/usr/bin", "USER=matys", "PWD=/home/matys", NULL};
    shell_t *shell = init_shell(env);
    int result;
    shell->args = malloc(sizeof(char *) * 3);
    shell->args[0] = strdup("unsetenv");
    shell->args[1] = NULL;
    shell->args[2] = NULL;
    result = fcts_unsetenv(shell);
    cr_assert_eq(shell->exit_status, 1);
}

Test(fcts_unsetenv, unsetenv_env_null)
{
    char **env = NULL;
    shell_t *shell = init_shell(env);
    int result;
    shell->args = malloc(sizeof(char *) * 3);
    shell->args[0] = strdup("unsetenv");
    shell->args[1] = strdup("USER");
    shell->args[2] = NULL;
    result = fcts_unsetenv(shell);
    cr_assert_eq(result, 1);
}

Test(fcts_unsetenv, unsetenv_users)
{
    char *env[] = {"PATH=/usr/bin", "USERR=matys", "PWD=/home/matys", NULL};
    shell_t *shell = init_shell(env);
    int result;
    shell->args = malloc(sizeof(char *) * 3);
    shell->args[0] = strdup("unsetenv");
    shell->args[1] = strdup("USER");
    shell->args[2] = NULL;
    result = fcts_unsetenv(shell);
    cr_assert_eq(result, 1);
}

Test(fcts_exit, should_return_0_exit)
{
    char *env[] = {"PATH=/usr/bin", "USER=matys", "PWD=/home/matys", NULL};
    shell_t *shell = init_shell(env);
    int result;
    shell->args = malloc(sizeof(char *) * 2);
    shell->args[0] = strdup("exit");
    shell->args[1] = NULL;
    result = fcts_exit(shell);
    cr_assert_eq(result, 0);
}

//Env
Test(env, normal)
{
    char *env[] = {"PATH=/usr/bin", "USER=matys", "PWD=/home/matys", NULL};
    shell_t *shell = init_shell(env);
    fcts_env(shell);
    cr_assert_eq(shell->exit_status, 0);
}

Test(env, value_null)
{
    char *env[] = {"PATH=/usr/bin", "USER=matys", "PWD=", NULL};
    shell_t *shell = init_shell(env);
    fcts_env(shell);
    cr_assert_eq(shell->exit_status, 0);
}

//lib1
Test(my_puterror, should_return_0)
{
    char *str = "Hello";
    int result = my_puterror(str);
    cr_assert_eq(result, 0);
}

Test(my_tablen, should_return_2)
{
    char *tab[] = {"Hello", "World", NULL};
    int result = my_tablen(tab);
    cr_assert_eq(result, 2);
}

Test(my_tablen, should_return_0)
{
    char **tab = NULL;
    int result = my_tablen(tab);
    cr_assert_eq(result, 0);
}

Test(my_tabdup, should_return_tab)
{
    char *tab[] = {"Hello", "World", NULL};
    char **result = my_tabdup(tab);
    cr_assert_not_null(result);
}

Test(my_tabdup, should_return_null)
{
    char **tab = NULL;
    char **result = my_tabdup(tab);
    cr_assert_null(result);
}

//getnbr
Test(my_getnbr, should_return_0)
{
    char *str = "Hello";
    int result = my_getnbr(str);
    cr_assert_eq(result, 0);
}

Test(my_getnbr, should_return_negativ)
{
    char *str = "-1";
    int result = my_getnbr(str);
    cr_assert_eq(result, -1);
}

Test(my_getnbr, should_return_1)
{
    char *str = "+1";
    int result = my_getnbr(str);
    cr_assert_eq(result, 1);
}

//builtins2.c
Test(fcts_history, should_return_0)
{
    char *env[] = {"PATH=/usr/bin", "USER=matys", "PWD=/home/matys", NULL};
    shell_t *shell = init_shell(env);
    shell->args = malloc(sizeof(char *) * 2);
    shell->args[0] = strdup("history");
    shell->args[1] = NULL;
    int result = fcts_history(shell);
    cr_assert_eq(result, 0);
}
