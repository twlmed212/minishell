NAME = minishell

SRC = ./src/ft_fnc/ft_strcmp.c ./src/ft_fnc/ft_perror.c ./src/signals/signals.c \
	 ./src/utils/env_utils.c  ./src/builtins/builtins.c ./src/builtins/exit.c \
	./src/builtins/env.c ./src/builtins/pwd.c ./src/builtins/export.c ./src/builtins/cd.c ./src/builtins/echo.c \
	./src/main.c ./src/execution/executor.c ./src/execution/pipes.c ./src/execution/redirections.c ./src/execution/heredoc.c \
	./src/parsing/parse_redirections.c ./src/parsing/parse_pipe.c ./src/parsing/quotes.c ./src/parsing/path.c \
	./src/builtins/unset.c ./src/parsing/parser.c 	./src/parsing/token.c ./src/parsing/syntax.c \
	./src/utils/executor_utils.c ./src/utils/executor_builtin.c ./src/utils/executor_exit_expand.c \
	./src/utils/executor_utils_pars.c ./src/utils/export_utils.c ./src/parsing/token_helper.c


OBJ = ${SRC:.c=.o}

CC = cc
CFLAGS = -Wall -Wextra  -Werror

LIBFT_DIR = ./libft
LIBFT = ${LIBFT_DIR}/libft.a
all: $(NAME)

${LIBFT}:
	@${MAKE} -C ${LIBFT_DIR}

$(NAME): $(OBJ) ${LIBFT}
	@$(CC) $(CFLAGS) $(OBJ) -o $(NAME)  ${LIBFT} -I/opt/homebrew/opt/readline/include -L/opt/homebrew/opt/readline/lib -lreadline

clean:
	@${MAKE} clean -C ${LIBFT_DIR}
	rm -f $(OBJ)

fclean: clean
	${MAKE} fclean -C ${LIBFT_DIR}
	rm -f $(NAME)

re: fclean all

run: all
	./minishell

.PHONY : all clean fclean re run