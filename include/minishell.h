/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtawil <mtawil@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 02:45:45 by mtawil            #+#    #+#             */
/*   Updated: 2025/11/27 13:59:13 by mtawil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
#include "../libft/libft.h"
#include <sys/stat.h>

// ============= DATA STRUCTURES =============
#define MAX_EXIT "255"
#define NAR_ERROR "1"
#define MIN_EXIT "0"

#ifndef TMA_ERROR
# define TMA_ERROR "1"
#endif

typedef enum e_redir_type
{
    REDIR_IN,
    REDIR_OUT,
    REDIR_APPEND,
    REDIR_HEREDOC,
    TOKEN_WORD,
    TOKEN_PIPE
}   t_redir_type;

typedef struct s_redir
{
    t_redir_type    type;
    char            *file;
    struct s_redir  *next;
}   t_redir;

typedef struct s_cmd
{
    char        **args;
    int         *saved_fds;
    t_redir     *redirs;
}   t_cmd;

typedef struct s_env_exit
{
    char    **env;
    int     last_exit;
}   t_env_and_exit;

typedef struct s_fd 
{
	int	fd_in;
	int	fd_out;
}	t_fd;


typedef struct s_tokens {
    char *value;
    int  flag;
    t_redir_type type;
    struct s_tokens *next;
} t_tokens;

extern volatile sig_atomic_t g_signal;
// ============ Signal handlers  =============
void    init_signals(void);
void    reset_signals(void);
void init_signals_child_exec(void);
// ============ TOKENIZER Moudnib =============
t_tokens *tokenize(const char *line, int *size);
int check_unclosed_quotes(const char *line);
int check_simple_command(t_tokens *tokens);
char *ft_strndup(const char *s, size_t n);
int ft_isspace(char c);
// ============= Pipe functions =============
char    ***split_all_pipes(char **args);
void    free_all_pipes(char ***cmds);
void    execute_pipeline(char ***cmds, t_env_and_exit *shell);
int     count_pipes(char **args);
int     has_pipe(char **args, t_env_and_exit *shell);
int     find_pipe(char **args);
void    execute_pipe(char **cmd1, char **cmd2, t_env_and_exit *shell);


// ============= Quote handling =============
int     is_quote(char c);
char    *remove_quotes(char *str);

// ============= EXECUTION =============

void execute_command(char *input, t_env_and_exit *shell);
char    **parse_command(char *input);
char *find_command_path(char *cmd, t_env_and_exit *shell);
void    free_array(char **arr);

// ============= BUILT-INS =============

int     is_builtin(char *cmd);
int	run_builtin(char **args, t_env_and_exit *shell);
int	builtin_echo(char **args);
int     builtin_pwd();
int builtin_env(char **args, t_env_and_exit *shell);
void builtin_exit(char **args, t_env_and_exit *shell);
int builtin_cd(char **args);
int     builtin_export(char **args, t_env_and_exit *shell);
int     builtin_unset(char **args, t_env_and_exit *shell);

// ============= ENVIRONMENT =============

char    **copy_env(char **environ);
char    *get_env_value(char *name, t_env_and_exit *shell);
int     set_env_value(char *name, char *value, t_env_and_exit *shell);
int     unset_env_value(char *name, t_env_and_exit *shell);

// ============= REDIRECTIONS =============

int         find_redir(char **args);
t_redir_type get_redir_type(char *op);
t_cmd       *parse_cmd_with_redir(char **args);
void        free_cmd(t_cmd *cmd);

int     *save_std_fds(void);
void    restore_std_fds(int *saved);
int     execute_output_redir(t_redir *redir);
int     execute_input_redir(t_redir *redir);
int     execute_redirections(t_redir *redirs);

// ============= HEREDOC =============

char	*read_heredoc(char *delimiter);
int	ft_strcmp(char *s1, char *s2);
void ft_perror(char *str);
#endif
