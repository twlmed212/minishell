#ifndef MINISHELL_H
#define MINISHELL_H

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <signal.h>
#include <fcntl.h>
#include <readline/readline.h>
#include <readline/history.h>

#define BUFFER_SIZE 1024

// ---------- ENUM ----------
typedef enum e_token_type {
    TOKEN_WORD,
    TOKEN_PIPE,
    TOKEN_REDIR_IN,      // <
    TOKEN_REDIR_OUT,     // >
    TOKEN_REDIR_APPEND,  // >>
    TOKEN_HEREDOC,        // <<
    TOKEN_INVALID
} t_token_type;

// ---------- STRUCTS ---------
typedef struct s_tokens {
    char *value;
    int  flag;
    t_token_type type;
    struct s_tokens *next;
} t_tokens;

typedef struct s_redirect {
    t_token_type type;
    char *filename;
    struct s_redirect *next;
} t_redirect;

typedef struct s_command {
    char **args;
    t_redirect *redirs;
    struct s_command *next;
} t_command;

typedef struct s_fd 
{
	int	fd_in;
	int	fd_out;
}	t_fd;

// ---------- FUNCTIONS ----------
// token.c
t_tokens *create_token(char *str, int fla);
void         add_token(t_tokens **head, t_tokens *new_token);
void         print_tokens(t_tokens *tokens);
int check_simple_command(t_tokens *tokens);
char *ft_substr(char const *s, unsigned int start, size_t len);
int ft_isspace(char c);
int ft_strcmp(const char *s1, const char *s2);
char	*ft_strdup(const char *s);
int check_unclosed_quotes(const char *line);
// parser.c
t_tokens *tokenize(const char *line);
t_command *new_command(int len);
void check_token(t_tokens *token);
t_command   *parse_tokens(t_tokens *tokens);
t_redirect  *new_redir(t_token_type type, char *file);
void         print_commands(t_command *cmds);

// optional utils
char        *clean_line(const char *line);
int          check_unclosed_quotes(const char *line);
char *ft_strndup(const char *s, size_t n);


// optional free
//void         free_tokens(t_token *tokens);
void         free_commands(t_command *cmds);

int ft_star_heredoc(t_tokens *tokens);

#endif
