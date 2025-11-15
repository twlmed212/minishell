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

void    execute_command(char **args, char **env);
char    **parse_command(char *input);
char    *find_command_path(char *cmd);
void    free_array(char **arr);

int     is_builtin(char *cmd);
int     execute_builtin(char **args, char **env);
int     builtin_echo(char **args);
int     builtin_pwd();
int     builtin_env(char **args);
int     builtin_exit(char **args);

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
