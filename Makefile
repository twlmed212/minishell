

NAME = minishell

SRC = src/main.c \
       src/tokenizer/tokenizer.c \
       src/tokenizer/tokenizer_utils.c \
       src/parsing/parser.c \
       src/parsing/parser_utils.c \
       src/parsing/quotes.c \
       src/execution/executor.c \
       src/execution/executor_utils.c \
       src/execution/pipes.c \
       src/execution/path.c \
       src/execution/heredoc.c \
       src/builtins/builtins.c \
       src/builtins/echo.c \
       src/builtins/pwd.c \
       src/builtins/cd.c \
       src/builtins/env.c \
       src/builtins/exit.c \
       src/builtins/export.c \
       src/builtins/unset.c \
       src/signals/signals.c \
       src/utils/ft_funcs.c \
       src/utils/export_utils.c \
       src/utils/memory.c \
       src/utils/file_descriptor.c \
       src/utils/env_set.c

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
