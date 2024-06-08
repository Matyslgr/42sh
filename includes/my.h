/*
** EPITECH PROJECT, 2024
** delivery
** File description:
** my
*/
#pragma once

#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stddef.h>
#include <signal.h>
#include <stdbool.h>
#include <time.h>
#include <ctype.h>

#define DEC_TREE_SIZE (--values->tree_size)
#define INC_TREE_SIZE (values->tree_size++)
#define DEC_STACK_SIZE (--values->stack_size)
#define INC_STACK_SIZE (values->stack_size++)
#define TIME_FORMAT "%H:%M"
#define IS_QUOTE(c) (c == '"' || c == '\'' || c == '`')
#define SHELL_NAME "42sh"
#define DISPLAY_ERROR perror(SHELL_NAME)
#define SHELL_PROMPT "$> "
typedef struct list {
    char *full_name;
    char *name;
    char *value;
    int oui;
    struct list *next;
} list_t;

typedef struct history_s {
    int index;
    char *time;
    char *command;
    struct history_s *next;
    struct history_s *prev;
} history_t;

typedef enum ast_node_type_e {
    OPERATOR,
    OPERAND
} ast_node_type_t;

typedef struct tree {
    char *value;
    enum ast_node_type_e type;
    struct tree *left;
    struct tree *right;
} tree_t;

typedef struct shell {
    char **args;
    char *prev_directory;
    int exit_status;
    int status;
    bool disp_tree;
    char *line;
    list_t *alias;
    list_t *env;
    list_t *local;
    history_t *history;
} shell_t;

typedef struct command {
    const char *name;
    int (*func)(shell_t *);
} command_t;

typedef struct list_char_s {
    char *data;
    struct list_char_s *next;
} list_char_t;

typedef struct tokenize_vals_s {
    char **tokens;
    int input_len;
    char *buffer;
    int tokens_index;
    int buffer_index;
} tokenize_vals_t;

typedef struct precedence_asso_s {
    int precedence;
    char op_str[3];
} precedence_asso_t;

typedef struct ast_val_s {
    char *token;
    char **tokens;
    int stack_size;
    int tree_size;
    tree_t **tree;
    tree_t **stack;
} ast_val_t;

typedef struct history_flags_s {
    bool c;
    bool r;
    bool h;
    int num;
} history_flags_t;

// lib1
int my_puterror(char *str);
int my_tablen(char **tab);
char **my_tabdup(char **tab);
char **my_tabcat(char **tab1, char **tab2);
int my_getnbr(const char *str);

// Strip str
char *strip_str(char *str);

// my_str_to_word_array
char **my_str_to_word_array(char *arg, char *delimiters);

// Split words 2
void add_word_to_array(char **words, char *str, int start, int end);
int loop(char *str, char *delimiters, char **words);

// SPlit words 3
void update_str(char *str, int i);

// unmatched quotes
int unmatched_quotes(char *str);

// Split words
char **split_words(char *str, char *delimiters, int remove_quotes);

// Strcatdup
char *strcatdup(char *str1, char *str2);

// Execute pipe
void execute_pipe(shell_t *Shell, tree_t *left, tree_t *right);

// Operators
void execute_redirect_stdout(shell_t *Shell, tree_t *left, tree_t *right);
void execute_redirect_stdout_append(shell_t *Shell, tree_t *left,
    tree_t *right);
void execute_redirect_stdin(shell_t *Shell, tree_t *left, tree_t *right);
void execute_here_doc(shell_t *Shell, tree_t *left, tree_t *right);
void execute_semicolon(shell_t *Shell, tree_t *left, tree_t *right);

// Operators2
void execute_and(shell_t *Shell, tree_t *left, tree_t *right);
void execute_or(shell_t *Shell, tree_t *left, tree_t *right);

// Execute command
char *get_command(shell_t *Shell);
int execute_command(shell_t *Shell);

// Tree
void execute_tree(shell_t *Shell, tree_t *tree);
void print_tree(tree_t *node, char *prefix, int is_left);
void execute_command_line(shell_t *Shell, char *command_line);

// setenv
int fcts_setenv(shell_t *Shell);

// Cd
int fcts_cd(shell_t *Shell);

// builtins
char *get_env_value(list_t *env, char *name);
int fcts_unsetenv(shell_t *Shell);
int fcts_exit(shell_t *Shell);
int fcts_env(shell_t *Shell);

// child process
void child_process(shell_t *Shell);

// minishell
shell_t *init_shell(char **env);
void add_node(list_t **list, char *name, char *value, char *alias);
void free_shell(shell_t *Shell);
void free_command(char **args);

// tokenize_command_line
char **tokenize_ast(const char *input);
bool is_operator(char input);

// Build_tree
tree_t *build_tree(char **tokens);

// Token_valid
bool is_token_valid(int i, const char *input);

// Dump buffer
bool dump_buffer(tokenize_vals_t *values, int i, const char *input);

// Alias
int fcts_alias(shell_t *Shell);
char *which_command(shell_t *Shell);
int fcts_unalias(shell_t *Shell);

// list
void delete_element(list_t **head, const char *name);
bool dump_buffer(tokenize_vals_t *values, int i, const char *input);

// globbings
char **my_globbings(char **input);

// builtin2
int fcts_set(shell_t *Shell);
int fcts_unset(shell_t *Shell);
int fcts_history(shell_t *Shell);
void free_history(history_t *history);
int my_vars(shell_t *Shell);
void edit_node(list_t **list, char *name, char *value);

// History
void save_command(shell_t *Shell, char **command);

//is_alias
int is_alias(shell_t *Shell);
int exec_alias(shell_t *Shell);

// Backticks
int backticks(tree_t *tree, shell_t *shell);

// init_alias
void init_alias(shell_t *Shell);
bool *backtic(void);

//termios
typedef struct hi_s {
    char **lineptr;
    FILE *stream;
    char user_input;
    size_t input_size;
    int cursor_position;
    size_t *n;
    shell_t *shell;
    int disp_cursor_pos_x;
    int term_size;
}hi_t;
typedef struct buffer_args_s {
    char **lineptr;
    size_t *n;
    size_t *input_size;
    char user_input;
    int *cursor_position;
} buffer_args_t;
void setup_termios(void);
int handle_input(char **lineptr, size_t *n, FILE *stream, shell_t *shell);
int get_cursor_position(shell_t *shell);
int get_terminal_width(void);
void update_cursor_position_right(hi_t *v);
void update_cursor_position_left(hi_t *v);
void insert_char_at_cursor(
    hi_t *v);
void delete_char_at_cursor(
    hi_t *v);
void display_prompt(shell_t *Shell);
void unset_termios(void);
#define INPUT_BUFFER_INCREMENT 1024
#define INPUT_DEFAULT_SEPARATOR '\n'
