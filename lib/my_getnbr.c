/*
** EPITECH PROJECT, 2024
** 42sh
** File description:
** my_getnbr
*/

int my_getnbr(const char *str)
{
    int result = 0;
    int sign = 1;

    if (*str == '-' || *str == '+') {
        sign = (*str == '-') ? -1 : 1;
        str++;
    }
    while (*str >= '0' && *str <= '9') {
        result = result * 10 + (*str - '0');
        str++;
    }
    return sign * result;
}
