NAME = minishell

SRC =  token.c minishell.c pars.c utilis.c syntax.c herdoc.c

OBJ = ${SRC:.c=.o}

CC = cc
CFLAGS = -Wall -Wextra # -Werror

# Rules for compiling object files
%.o:%.c headerfile.h
	$(CC) $(CFLAGS) -c $< -o $@

# Rule to create static library
$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME) -lreadline

# Clean object files
clean:
	rm -f $(OBJ)

# Clean all generated files
fclean: clean
	rm -f $(NAME)

# Rebuild all
re: fclean all

# Default target
all: $(NAME)

m: all
	./minishell

.PHONY : all clean fclean re
