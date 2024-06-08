/*
** EPITECH PROJECT, 2024
** B-PSU-200-MPL-2-1-minishell2-matys.laguerre
** File description:
** split_words
*/

#include "my.h"

static void is_word(int *count, int consecutive_delimiters)
{
    if (!consecutive_delimiters) {
        (*count)++;
    }
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

static int mini_loop(char *str, char *delimiters)
{
    int count = 0;
    int in_quotes = 0;
    int consecutive_delimiters = 0;

    for (int i = 0; str[i] != '\0'; i++) {
        if (check_in_quotes(str[i], &in_quotes))
            continue;
        if (!in_quotes && strchr(delimiters, str[i]) != NULL &&
            (i == 0 || str[i - 1] != '\\')) {
            is_word(&count, consecutive_delimiters);
            consecutive_delimiters = 1;
        } else
            consecutive_delimiters = 0;
    }
    return count;
}

static int count_words(char *str, char *delimiters)
{
    int count;

    while (*str != '\0' && strchr(delimiters, *str) != NULL)
        str++;
    count = mini_loop(str, delimiters);
    if (str[0] != '\0')
        count++;
    return count;
}

static char **allocate_memory(int count)
{
    char **words = (char **)malloc((count + 1) * sizeof(char *));

    for (int i = 0; i < count + 1; i++)
        words[i] = NULL;
    return words;
}

// remove quotes from words
static void decale_str(char **word, int last_index, int current_index)
{
    char *new_word = malloc(sizeof(char) * strlen(*word) - 1);
    int i = 0;

    for (; (*word)[i] != '\0' && i < last_index; i++)
        new_word[i] = (*word)[i];
    for (int j = last_index + 1; (*word)[j] != '\0' &&
        j < current_index; j++) {
        new_word[i] = (*word)[j];
        i++;
    }
    for (int j = current_index + 1; (*word)[j] != '\0'; j++) {
        new_word[i] = (*word)[j];
        i++;
    }
    new_word[i] = '\0';
    free(*word);
    *word = new_word;
}

static void check_decale_str(bool check, char **word, int last_index,
    int current_index)
{
    if (check)
        decale_str(word, last_index, current_index);
}

static void process_remove_quotes(char **word)
{
    int simple = 0;
    int double_ = 0;
    int last_quote = 0;

    for (int i = 0; (*word)[i] != '\0'; i++) {
        if ((*word)[i] == '"' && !simple) {
            check_decale_str(double_, word, last_quote, i);
            double_ = !double_;
            last_quote = i;
        }
        if ((*word)[i] == '\'' && !double_) {
            check_decale_str(simple, word, last_quote, i);
            simple = !simple;
            last_quote = i;
        }
    }
}

static void remove_quotes_from_words(char **words)
{
    for (int i = 0; words[i] != NULL; i++)
        process_remove_quotes(&words[i]);
}

char **split_words(char *str, char *delimiters, int remove_quotes)
{
    int count;
    char **words;
    int start;

    if (unmatched_quotes(str))
        return NULL;
    count = count_words(str, delimiters);
    words = allocate_memory(count);
    start = loop(str, delimiters, words);
    if (start < (int)strlen(str))
        add_word_to_array(words, str, start, strlen(str));
    if (remove_quotes)
        remove_quotes_from_words(words);
    return words;
}
