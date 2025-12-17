
NAME = minishell

SRC = src/main.c \
       src/utils/main_utils.c \
       src/tokenizer/tokenizer.c \
       src/tokenizer/tokenizer_utils.c \
       src/parsing/parser.c \
       src/parsing/parser_utils.c \
       src/parsing/parser_utils_2.c \
       src/parsing/quotes.c \
       src/execution/executor.c \
       src/execution/executor_utils.c \
       src/execution/pipes.c \
       src/execution/path.c \
       src/execution/heredoc.c \
       src/execution/heredoc_utils.c \
       src/builtins/builtins.c \
       src/builtins/echo.c \
       src/builtins/pwd.c \
       src/builtins/cd.c \
       src/builtins/env.c \
       src/builtins/exit.c \
       src/builtins/export.c \
       src/builtins/unset.c \
       src/signals/signals.c \
       src/signals/heredoc_signals.c \
       src/utils/ft_funcs.c \
       src/utils/export_utils.c \
       src/utils/file_descriptor.c \
       src/utils/env_set.c \
       src/cleaner/cleanup.c \
       src/utils/env_get.c

OBJ_DIR = obj
OBJ = ${SRC:src/%.c=${OBJ_DIR}/%.o}

CC = cc
CFLAGS = -Wall -Wextra  -Werror 

LIBFT_DIR = ./libft
LIBFT = ${LIBFT_DIR}/libft.a

all: $(NAME)

${LIBFT}:
	@${MAKE} -C ${LIBFT_DIR}

$(NAME): $(OBJ) ${LIBFT}
	@$(CC) $(CFLAGS) $(OBJ) -o $(NAME)  ${LIBFT} -lreadline

$(OBJ_DIR)/%.o: src/%.c
	@mkdir -p $(OBJ_DIR)/utils $(OBJ_DIR)/tokenizer $(OBJ_DIR)/parsing \
			$(OBJ_DIR)/execution $(OBJ_DIR)/builtins $(OBJ_DIR)/signals \
			$(OBJ_DIR)/cleaner
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@${MAKE} clean -C ${LIBFT_DIR}
	rm -f $(OBJ)
	rm -rf $(OBJ_DIR)

fclean: clean
	${MAKE} fclean -C ${LIBFT_DIR}
	rm -f $(NAME)

re: fclean all

run: all
	./minishell

.PHONY : all clean fclean re run
