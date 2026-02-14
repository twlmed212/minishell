/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtawil <mtawil@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 02:45:45 by mtawil            #+#    #+#             */
/*   Updated: 2026/02/14 18:03:27 by mtawil           ###   ########.fr       */
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

// ============= Enums =============
typedef enum e_redir_type
{
	T_REDIR_IN,
	T_REDIR_OUT,
	T_APPEND,
	T_HEREDOC
}								t_redir_type;

// ============= Structures =============
typedef struct s_redir
{
	t_redir_type				type;
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

// ============= Garbeg Collectors =============

typedef struct s_gc
{
	void						*data;
	struct s_gc					*next;
}								t_gc;

void							*ft_malloc(size_t size);
void							free_grabage(void);

// ============= Globals =============
extern volatile sig_atomic_t	g_signal;
# define LLONG_MAX 9223372036854775807LL
# define LLONG_MIN -9223372036854775808LL

// ============= Main Utils =============
t_shell							*get_and_set_value(t_shell *original, int code);
int								process_line(char *line, t_shell *shell);
int								check_signals(char *line, t_shell *shell);

// ============= Parser =============
t_cmd							*parser(char *str, t_cmd **cmd_list,
									t_cmd **current);
char							*extract_word(char *str, int *i);
void							skip_spaces(char *str, int *i);
int								is_special_char(char c);
int								check_syntax_error(char *str);
t_redir_type					get_redir_type(char *str, int *i);
t_cmd							*alloc_cmd(void);
t_cmd							*alloc_cmd(void);
void							add_cmd(t_cmd **cmd_list, t_cmd *new_cmd);
int								add_arg(t_cmd *cmd, char *arg);

// ============= Qoutes =============
char							*remove_qoutes(char *str);
int								check_unclosed_qoutes(char *line);

// ============= Hererdoc Utils =============
char							*handle_heredoc(char *delimiter);
int								write_to_file(char *input, char *delimiter,
									int fd);
int								prepare_file(char **filename, int *fd);

// ============= EXECUTOR =============
void							executor(t_cmd *cmds, t_shell *shell);
char							*find_path(char *cmd, char **env);
void							exec_pipeline(t_cmd *cmds, t_shell *shell,
									int **pipes, int n);
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
int								get_fd_file(int new);

// ============= UTILS =============
char							**copy_env(char **env);
char							*get_env(char *key, char **env);
int								set_env(char *key, char *value, t_shell *shell);
int								ft_strcmp(const char *s1, const char *s2);
char							*get_name(char *arg, t_shell *shell, int *ret,
									int *i);
char							*get_value(char *arg, char *name, int *ret,
									int *i);
void							ft_perror(char *s);

// ============= PIPES =============
int								count_cmds(t_cmd *cmds);
void							setup_pipes(int **pipes, int i, int n);
void							free_pipes(int **pipes, int n, int flag);
void							close_pipes(int **pipes, int n);
int								**create_pipes(int n);

// ============= EXECUTOR UTILS =============
void							exec_cmd(t_cmd *cmd, t_shell *shell,
									char **env);
int								handle_redirs(t_redir *redirs);
void							disable_parent_signals(void);

// ============= EXPAND =============
char							*expand(char *cmds);
int								ft_isspace(char ch);
int								var_name_len(char *str);
char							*var_value(char *after_dollar, int len,
									t_shell *shell);
int								count_expanded_len(char *str, t_shell *shell);

#endif