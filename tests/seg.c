/*
** EPITECH PROJECT, 2024
** Shell_Programming
** File description:
** seg
*/
#include <signal.h>

int main(void)
{
    raise(SIGSEGV);
    return 0;
}
