/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtawil <mtawil@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 15:34:17 by mtawil            #+#    #+#             */
/*   Updated: 2025/12/06 18:17:29 by mtawil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static char	*generate_unique_tempfile(void)
{
	static int	counter = 0;
	char		*num_str;
	char		*temp1;
	char		*filename;

	num_str = ft_itoa(counter++);
	if (!num_str)
		return (NULL);
	temp1 = ft_strjoin("/tmp/.heredoc_temp_", num_str);
	free(num_str);
	if (!temp1)
		return (NULL);
	num_str = ft_itoa(getpid());
	if (!num_str)
	{
		free(temp1);
		return (NULL);
	}
	filename = ft_strjoin(temp1, num_str);
	free(temp1);
	free(num_str);
	return (filename);
}

static int	write_to_file(char *input, char *delimiter, int fd)
{
	if (!input)
		return (1);
	if (ft_strcmp(input, delimiter) == 0)
	{
		free(input);
		return (1);
	}
	write(fd, input, ft_strlen(input));
	write(fd, "\n", 1);
	free(input);
	return (0);
}

static int	prepare_file(char **filename)
{
	int	fd;
	*filename = generate_unique_tempfile();
	if (!*filename)
		return (1);
	fd = open(*filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
		return (close(fd), perror("heredoc"), free(*filename), 1);
	close(fd);
	return (0);
}
int *child_fd(int *fd){
	static int *new;
	if (fd)
		new = fd;
	return (new);
}
void	handle_sigint_heredoc(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	close(*child_fd(NULL));
	exit(130);
}
void	herdoc_child(char *filename, char *del)
{
	char	*input;
	int		fd;

	child_fd(&fd);
	signal(SIGINT, handle_sigint_heredoc);
	signal(SIGQUIT, SIG_IGN);
	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1)
    {
        perror("heredoc");
        exit(1);
    }
	while (1)
	{
		input = readline("> ");
		if (g_signal == SIGINT)
		{
			if (input)
				free(input);
			g_signal = 0;
			close(fd);
			exit(130);
		}
		if (write_to_file(input, del, fd))
			break ;
	}
	close(fd);
	exit(0);
}

void	herdoc_parent(pid_t pid, char *fn)
{
	int	wstatus;
	int	status;

	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	waitpid(pid, &wstatus, 0);
	status = WEXITSTATUS(wstatus);
	if (status == 130 || status == 1)
	{
		if (status == 130)
			get_and_set_value(NULL, 130);
		else
			get_and_set_value(NULL, 1);
		unlink(fn);
		free(fn);
		return ;
	}
	get_and_set_value(NULL, 0);
	return ;
}

char	*read_heredoc(char *delimiter)
{
	char			*filename;
	pid_t			pid;
	t_env_and_exit	*shell;

	if (prepare_file(&filename))
		return (NULL);
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		unlink(filename);
		return (free(filename), NULL);
	}
	if (pid == 0)
		herdoc_child(filename, delimiter);
	else
		herdoc_parent(pid, filename);
	shell = get_and_set_value(NULL, -1);
	if (shell->last_exit == 130)
		return (free(filename), NULL);
	return (filename);
}
