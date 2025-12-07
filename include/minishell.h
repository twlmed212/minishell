#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <signal.h>
# include <sys/wait.h>
# include <fcntl.h>
# include "../libft/libft.h"
# include <readline/readline.h>
# include <readline/history.h>
# include <errno.h>

/* ============= CONSTANTS ============= */
# define PROMPT "minishell> "

/* ============= ENUMS ============= */
typedef enum e_token_type
{
	T_WORD,
	T_PIPE,
	T_REDIR_IN,
	T_REDIR_OUT,
	T_APPEND,
	T_HEREDOC
}	t_token_type;

/* ============= STRUCTURES ============= */
typedef struct s_token
{
	char			*value;
	t_token_type	type;
	struct s_token	*next;
}	t_token;

typedef struct s_redir
{
	t_token_type	type;
	char			*file;
	struct s_redir	*next;
}	t_redir;

typedef struct s_cmd
{
	char			**args;
	t_redir			*redirs;
	struct s_cmd	*next;
}	t_cmd;

typedef struct s_shell
{
	char			**env;
	int				exit_code;
	int				running;
}	t_shell;

typedef struct s_quotes
{
	char						*result;
	int							i;
	int							j;
	char						quote;
}								t_quotes;


typedef struct s_to_free {
	char	*filename;
	t_shell	*shell;
	t_cmd	*cmds;
} t_to_free;

/* ============= GLOBALS ============= */
extern volatile sig_atomic_t	g_signal;

// File Discriptor Utils
int	*save_std_fds(void);
void	restore_std_fds(int *saved);
// hererdoc
char	*handle_heredoc(char *delimiter);
char	*expand_exit_code(char *cmds);
t_shell *get_and_set_value(t_shell *original, int code);
t_cmd *get_pointer_cmds(t_cmd *original);
/* ============= LEXER ============= */
t_token		*lexer(char *line);
void		free_tokens(t_token *tokens);

/* ============= PARSER ============= */
t_cmd		*parser(t_token *tokens);
void		free_cmds(t_cmd *cmds);

/* ============= EXECUTOR ============= */
void		executor(t_cmd *cmds, t_shell *shell);
char		*find_path(char *cmd, char **env);

/* ============= BUILTINS ============= */
int			is_builtin(char *cmd);
int			exec_builtin(t_cmd *cmd, t_shell *shell);
int			builtin_echo(char **args, t_shell *shell);
int			builtin_cd(char **args, t_shell *shell);
int			builtin_pwd(void);
int	builtin_export(char **args, t_shell *shell);
int			builtin_unset(char **args, t_shell *shell);
int			builtin_env(t_shell *shell);
void		builtin_exit(char **args, t_shell *shell, t_cmd *cmd);
/* ============= SIGNALS ============= */
void		setup_signals(void);
void		handle_sigint(int sig);

/* ============= UTILS ============= */
char		**copy_env(char **env);
char		*get_env(char *key, char **env);
int			set_env(char *key, char *value, t_shell *shell);
void		free_array(char **arr);
int			ft_strcmp(const char *s1, const char *s2);


int		set_env(char *key, char *value, t_shell *shell);
char	*get_name(char *arg, t_shell *shell, int *ret, int *i);
char	*get_value(char *arg, char *name, int *ret, int *i);
char	*get_value(char *arg, char *name, int *ret, int *i);
int	is_var_valid(char c, int is_first);

char	*remove_quotes(char *str);
int		check_unclosed_quotes(char *line);
void	ft_perror(char *s);

int		count_cmds(t_cmd *cmds);
void	setup_pipes(int **pipes, int i, int n);
void	close_pipes(int **pipes, int n);
int		**create_pipes(int n);
void	exec_cmd(t_cmd *cmd, t_shell *shell, char **env);
int		handle_redirs(t_redir *redirs);
void	init_signals_child_exec(void);
void	exec_cmd(t_cmd *cmd, t_shell *shell, char **env);
int		handle_redirs(t_redir *redirs);

#endif