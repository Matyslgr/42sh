/*
** EPITECH PROJECT, 2024
** B-PSU-200-MPL-2-1-minishell2-matys.laguerre
** File description:
** split_string2
*/

#include "my.h"

struct info_s {
    int consecutive_delimiters;
    int start;
    char *str;
};

static void extract_word(char *str, int start, int end, char *word)
{
    int length = end - start;

    strncpy(word, str + start, length);
    word[length] = '\0';
}

void add_word_to_array(char **words, char *str, int start, int end)
{
    int word_index = 0;
    char *word = (char *)malloc((end - start + 1) * sizeof(char));

    while (words[word_index] != NULL)
        word_index++;
    extract_word(str, start, end, word);
    words[word_index] = word;
}

static void handle_delimiter(char **words, int index, struct info_s *info)
{
    if (!info->consecutive_delimiters) {
        add_word_to_array(words, info->str, info->start, index);
        info->start = index + 1;
    }
    info->consecutive_delimiters = 1;
}

static void handle_non_delimiter(struct info_s *info, int index)
{
    if (info->consecutive_delimiters) {
        (info->start) = IS_QUOTE(info->str[index - 1]) ? (index - 1) : index;
    }
    info->consecutive_delimiters = 0;
}

static int check_in_quotes(char c, int *in_quotes)
{
    static bool single = false;
    static bool doble = false;
    static bool backticks = false;

    if (c == '"' && !single && !backticks) {
        doble = !doble;
        *in_quotes = !*in_quotes;
        return 1;
    }
    if (c == '\'' && !doble && !backticks) {
        single = !single;
        *in_quotes = !*in_quotes;
        return 1;
    }
    if (c == '`' && !doble && !single) {
        backticks = !backticks;
        *in_quotes = !*in_quotes;
        return 1;
    }
    return 0;
}

static bool is_delimiter(char *str, int i, char *delimiters)
{
    if (strchr(delimiters, str[i]) != NULL)
        return true;
    if (str[i] == '\\' && str[i + 1] == 't')
        return true;
    if (str[i] == '\\' && str[i + 1] == 'n')
        return true;
    return false;
}

static void remove_char(char *str, int index)
{
    int i = index;

    for (; str[i] != '\0'; i++)
        str[i] = str[i + 1];
    str[i] = '\0';
}

static bool check_backslash(struct info_s *info, int i, int in_quotes,
    char *delimiters)
{
    if (!in_quotes && info->str[i] == '\\' &&
        is_delimiter(info->str, i, delimiters)) {
        remove_char(info->str, i);
        handle_non_delimiter(info, i);
        return true;
    }
    if (!in_quotes && info->str[i] == '\\') {
        remove_char(info->str, i);
        return true;
    }
    return false;
}

static void browse_str(char **words, char *delimiters, struct info_s *info)
{
    int in_quotes = 0;

    for (int i = 0; info->str[i] != '\0'; i++) {
        if (check_in_quotes(info->str[i], &in_quotes))
            continue;
        if (check_backslash(info, i, in_quotes, delimiters))
            continue;
        if (!in_quotes && info->str[i] != '\\' &&
            (strchr(delimiters, info->str[i]) != NULL ||
            IS_QUOTE(info->str[i])))
            handle_delimiter(words, i, info);
        else
            handle_non_delimiter(info, i);
    }
}

int loop(char *str, char *delimiters, char **words)
{
    struct info_s *info = malloc(sizeof(struct info_s));
    int start = 0;

    info->consecutive_delimiters = 0;
    info->start = 0;
    info->str = str;
    browse_str(words, delimiters, info);
    start = info->start;
    free(info);
    return start;
}
