/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtawil <mtawil@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 02:45:45 by mtawil            #+#    #+#             */
/*   Updated: 2025/11/17 01:45:08 by mtawil           ###   ########.fr       */
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

typedef enum e_redir_type
{
    REDIR_IN,
    REDIR_OUT,
    REDIR_APPEND,
    REDIR_HEREDOC
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
    t_redir     *redirs;
}   t_cmd;

typedef struct s_shell
{
    char    **env;
    int     last_exit;
}   t_shell;


void execute_command(char *input, t_shell *shell);
char    **parse_command(char *input);
char *find_command_path(char *cmd, t_shell *shell);
void    free_array(char **arr);

int     is_builtin(char *cmd);
int execute_builtin(char **args, t_shell *shell);
int     builtin_echo(char **args);
int     builtin_pwd();
int builtin_env(char **args, t_shell *shell);
int     builtin_exit(char **args);
int builtin_cd(char **args);
int     builtin_export(char **args, t_shell *shell);
int     builtin_unset(char **args, t_shell *shell);
int     builtin_exit(char **args);

char    **copy_env(char **environ);
char    *get_env_value(char *name, t_shell *shell);
int     set_env_value(char *name, char *value, t_shell *shell);
int     unset_env_value(char *name, t_shell *shell);

int         find_redir(char **args);
t_redir_type get_redir_type(char *op);
t_cmd       *parse_cmd_with_redir(char **args);
void        free_cmd(t_cmd *cmd);

int     *save_std_fds(void);
void    restore_std_fds(int *saved);
int     execute_output_redir(t_redir *redir);
int     execute_input_redir(t_redir *redir);
int     execute_redirections(t_redir *redirs);

// LibFt Functions :

int		ft_atoi(const char *nptr);
void	ft_bzero(void *s, int n);
void	*ft_calloc(size_t nmemb, size_t size);
int		ft_isalnum(int d);
int		ft_isalpha(int c);
int		ft_isascii(int c);
int		ft_isdigit(int d);
int		ft_isprint(int c);
char	*ft_itoa(int n);
void	*ft_memchr(const void *s, int c, size_t n);
int		ft_memcmp(const void *s1, const void *s2, size_t n);
void	*ft_memcpy(void *dest_str, const void *src_str, size_t n);
void	*ft_memmove(void *dst, const void *src, size_t len);
void	*ft_memset(void *a, int c, int len);
void	ft_putchar_fd(char c, int fd);
void	ft_putendl_fd(char *s, int fd);
void	ft_putnbr_fd(int n, int fd);
void	ft_putstr_fd(char *s, int fd);
char	**ft_split(char const *s, char c);
char	*ft_strchr(const char *s, int c);
char	*ft_strdup(const char *s);
void	ft_striteri(char *s, void (*f)(unsigned int, char *));
char	*ft_strjoin(char const *s1, char const *s2);
size_t	ft_strlcat(char *dst, const char *src, size_t size);
size_t	ft_strlcpy(char *dst, const char *src, size_t size);
size_t	ft_strlen(const char *s);
char	*ft_strmapi(char const *s, char (*f)(unsigned int, char));
int		ft_strncmp(const char *s1, const char *s2, size_t n);
char	*ft_strnstr(const char *big, const char *little, size_t len);
char	*ft_strrchr(const char *s, int c);
char	*ft_strtrim(char const *s1, char const *set);
char	*ft_substr(char const *s, unsigned int start, size_t len);
int		ft_tolower(int c);
int		ft_toupper(int c);

#endif
