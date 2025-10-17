#include "minishell.h"

void	ft_putchar_fd(char c, int fd)
{
	write(fd, &c, 1);
}

static char	*ft_char(char *s, unsigned int number, int len)
{
	while (number > 0)
	{
		s[len--] = 48 + (number % 10);
		number = number / 10;
	}
	return (s);
}
void	ft_putstr_fd(char *s, int fd)
{
	int	i;

	if (!s)
		return ;
	i = 0;
	while (s[i])
	{
		write(fd, &s[i], 1);
		i++;
	}
}

static int	get_num_length(int n)
{
	int	length;

	if (n == 0)
		return (1);
	length = 0;
	if (n < 0)
		length++;
	while (n != 0)
	{
		length++;
		n /= 10;
	}
	return (length);
}

char	*ft_itoa(int n)
{
	int		length;
	char	*str;
	long	nb;

	nb = n;
	length = get_num_length(n);
	str = (char *)malloc(length + 1);
	if (!str)
		return (NULL);
	str[length--] = '\0';
	if (nb < 0)
	{
		str[0] = '-';
		nb = -nb;
	}
	else if (nb == 0)
	{
		str[0] = '0';
	}
	str = ft_char(str, nb, length);
	return (str);
}
char *generate_random_name(void)
{
	int fd;
	char *filename;
	int rund[4];
	
	fd = open("/dev/random", O_RDONLY);
	read(fd, rund, 4);
	close(fd);
	fd = rund[0] + rund[1] + rund[2] + rund[3];
	filename = ft_itoa(fd);
	return (filename);
}

int create_fd(char *filename, t_fd *fd)
{
	fd->fd_in = open(filename, O_CREAT | O_TRUNC | O_WRONLY, 0644);
	if (fd->fd_in < 0)
		return (-1);
	fd->fd_out = open(filename, O_RDONLY);
	if (fd->fd_out < 0)
		return (close(fd->fd_in), -1);
	unlink(filename);
	return (0);
}

void heredoc_handler(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	exit(130);
}


int full_heredoc(t_fd fd, char *delim)
{
	int child;
	int status;
	char *line;

	child = fork();
	if (child < 0)
		return (-1);
	if (child == 0)
	{
		signal(SIGINT, heredoc_handler);
		while (1)
		{
			line = readline("> ");
			if (!line)
				break;
			if (ft_strcmp(line, delim) == 0)
			{
				free(line);
				break;
			}
			ft_putstr_fd(line, fd.fd_in);
			ft_putchar_fd('\n', fd.fd_in);
			free(line);
		}
		close(fd.fd_in);
		exit(0);
	}
	else
	{
		signal(SIGINT, SIG_IGN);
		waitpid(child, &status, 0);
		status = WEXITSTATUS(status);
		if (status == 130)
			return (close(fd.fd_out), -2);
	}
	return (fd.fd_out);
}

int heredoc(char *delim)
{
	char *filename;
	int fd_final;
	t_fd fd;

	filename = generate_random_name();
	if (!filename)
		return (-1);
	if (create_fd(filename, &fd) < 0)
		return (free(filename), -1);
	free(filename);
	fd_final = full_heredoc(fd, delim);
	close(fd.fd_in);
	return (fd_final);
}

int ft_star_heredoc(t_tokens *tokens){

	t_tokens *tmp = tokens;

	while (tmp)
	{
		if (tmp->type == TOKEN_HEREDOC)
		{
			tmp = tmp->next;
			if (heredoc(tmp->value) == -1)
				return (-1);
		}
		tmp = tmp->next;
	}
    return 0;
}