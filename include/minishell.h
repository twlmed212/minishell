/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtawil <mtawil@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 02:45:45 by mtawil            #+#    #+#             */
/*   Updated: 2025/12/17 23:43:55 by mtawil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <errno.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/wait.h>
# include <unistd.h>

// ============= ENUMS =============
typedef enum e_token_type
{
	T_WORD,
	T_PIPE,
	T_REDIR_IN,
	T_REDIR_OUT,
	T_APPEND,
	T_HEREDOC
}								t_token_type;

// ============= STRUCTURES =============
typedef struct s_token
{
	char						*value;
	t_token_type				type;
	struct s_token				*next;
}								t_token;

typedef struct s_redir
{
	t_token_type				type;
	char						*file;
	struct s_redir				*next;
}								t_redir;

typedef struct s_cmd
{
	char						**args;
	t_redir						*redirs;
	struct s_cmd				*next;
}								t_cmd;

typedef struct s_shell
{
	char						**env;
	int							exit_code;
	int							running;
}								t_shell;

typedef struct s_quotes
{
	char						*result;
	int							i;
	int							j;
	char						quote;
}								t_quotes;

typedef struct s_to_free
{
	char						*filename;
	t_shell						*shell;
	t_cmd						*cmds;
}								t_to_free;

typedef struct s_gc
{
	void				*data;
	struct s_gc			*next;
}						t_gc;

void							*ft_malloc(size_t size);
void							free_grabage(void);

// ============= GLOBALS =============
extern volatile sig_atomic_t	g_signal;
# define LLONG_MAX 9223372036854775807LL
# define LLONG_MIN -9223372036854775808LL
// ============= Main Utils =============
t_cmd							*get_pointer_cmds(t_cmd *original);
t_shell							*get_and_set_value(t_shell *original, int code);
int								process_line(char *line, t_shell *shell);
int								check_signals(char *line, t_shell *shell);

// ============= Hererdoc Utils =============
char							*handle_heredoc(char *delimiter);
char							*expand_exit_code(char *cmds);
int								write_to_file(char *input, char *delimiter,
									int fd);
t_cmd							*get_pointer_cmds(t_cmd *original);
int								prepare_file(char **filename, int *fd);

// ============= File Discriptor Utils =============
//
int								*save_std_fds(void);
void							restore_std_fds(int *saved);
// ============= LEXER =============
t_token							*toknizer(char *line);

// ============= PARSER =============
t_cmd							*parser(t_token *tokens);
t_redir							*process_redir(t_token *token);
char							*expand_exit_code(char *cmds);
int								handle_pipe_token(t_token **tokens, int flag);
t_cmd							*handle_parse_error(t_cmd *cmds, t_cmd *new);
int								check_syntax(t_token *token);
// ============= EXECUTOR =============
void							executor(t_cmd *cmds, t_shell *shell);
char							*find_path(char *cmd, char **env);

// ============= BUILTINS =============
int								is_builtin(char *cmd);
int								exec_builtin(t_cmd *cmd, t_shell *shell);
int								builtin_echo(char **args);
int								builtin_cd(char **args, t_shell *shell);
int								builtin_pwd(void);
int								builtin_export(char **args, t_shell *shell);
int								builtin_unset(char **args, t_shell *shell);
int								builtin_env(t_shell *shell);
void							builtin_exit(t_cmd *cmd, t_shell *shell);
// ============= SIGNALS =============
void							setup_signals(void);
void							restore_signals(void);
void							handle_sigint(int sig);
void							handle_sigint_heredoc(int sig);

// ============= UTILS =============
char							**copy_env(char **env);
char							*get_env(char *key, char **env);
int								set_env(char *key, char *value, t_shell *shell);
int								ft_strcmp(const char *s1, const char *s2);

int								set_env(char *key, char *value, t_shell *shell);
char							*get_name(char *arg, t_shell *shell, int *ret,
									int *i);
char							*get_value(char *arg, char *name, int *ret,
									int *i);
char							*remove_quotes(char *str);
int								check_unclosed_quotes(char *line);
void							ft_perror(char *s);

int								count_cmds(t_cmd *cmds);
void							setup_pipes(int **pipes, int i, int n);
void							free_pipes(int **pipes, int n, int flag);
void							close_pipes(int **pipes, int n);
int								**create_pipes(int n);
void							exec_cmd(t_cmd *cmd, t_shell *shell,
									char **env);
int								handle_redirs(t_redir *redirs);
void							disable_parent_signals(void);
void							exec_cmd(t_cmd *cmd, t_shell *shell,
									char **env);
int								handle_redirs(t_redir *redirs);

#endif