NAME = minishell

SRC = ./src/ft_fnc/ft_strcmp.c ./src/signals/signals.c ./src/utils/expand_str.c ./src/utils/get_var_values.c \
	./src/utils/expand_args.c ./src/utils/env_utils.c ./src/utils/expand_vars.c ./src/builtins/builtins.c ./src/builtins/exit.c \
	./src/builtins/env.c ./src/builtins/pwd.c ./src/builtins/export.c ./src/builtins/cd.c ./src/builtins/echo.c \
	./src/main.c ./src/execution/executor.c ./src/execution/pipes.c ./src/execution/redirections.c ./src/execution/heredoc.c \
	./src/parsing/parse_redirections.c ./src/parsing/parse_pipe.c ./src/parsing/quotes.c ./src/parsing/path.c \
	./src/builtins/unset.c ./src/parsing/parser.c

OBJ = ${SRC:.c=.o}

CC = cc
CFLAGS = -Wall -Wextra  -Werror

LIBFT_DIR = ./libft
LIBFT = ${LIBFT_DIR}/libft.a
all: $(NAME)

${LIBFT}:
	${MAKE} -C ${LIBFT_DIR}

$(NAME): $(OBJ) ${LIBFT}
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)  ${LIBFT} -I/opt/homebrew/opt/readline/include -L/opt/homebrew/opt/readline/lib -lreadline

clean:
	${MAKE} clean -C ${LIBFT_DIR}
	rm -f $(OBJ)

fclean: clean
	${MAKE} fclean -C ${LIBFT_DIR}
	rm -f $(NAME)

re: fclean all

run: all
	./minishell

.PHONY : all clean fclean re run