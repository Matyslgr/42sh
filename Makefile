##
## EPITECH PROJECT, 2024
## delivery
## File description:
## Makefile
##

NAME	=	42sh

NAME_TESTS	=	unit_tests


SRCS    :=    $(shell find -name "*.c"    \
        -not -path "./tests/*")

SRCS_TESTS	=	src/builtins.c \
				src/cd.c \
				lib/lib1.c \
				src/minishell.c \
				src/builtins2.c \
				src/builtin2.c \
				src/init_alias.c \
				lib/my_getnbr.c	\
				tests/test.c \
				lib/my_str_to_word_array.c	\
				src/handle_inputs/setup_termios.c


OBJ	=	$(SRCS:.c=.o)

CFLAGS	+=	-Wall -Wextra -Werror

LD_FLAGS	=	-lcriterion --coverage

CPPFLAGS	=	-Iincludes

all:	$(NAME)

$(NAME):	$(OBJ)
	@$(CC) -o $(NAME) $(OBJ) $(CPPFLAGS)

tests_run:
	@$(CC) -o $(NAME_TESTS) $(SRCS_TESTS) $(CPPFLAGS) $(LD_FLAGS)
	@./$(NAME_TESTS)

clean:
	@$(RM) $(OBJ)
	@$(RM) *.gc*

fclean:	clean
	@$(RM) $(NAME)
	@$(RM) $(NAME_TESTS)

re:	fclean all
