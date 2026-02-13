/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtawil <mtawil@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 15:34:17 by mtawil            #+#    #+#             */
/*   Updated: 2026/02/13 15:57:28 by mtawil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_fd_file(int new)
{
	static int	fd;

	if (new != -1)
		fd = new;
	return (fd);
}

static void	heredoc_child(int *fd, char *delimiter)
{
	char	*line;

	signal(SIGINT, handle_sigint_heredoc);
	signal(SIGQUIT, SIG_IGN);
	get_fd_file(*fd);
	while (1)
	{
		line = readline("> ");
		if (write_to_file(line, delimiter, *fd))
			break ;
	}
	close(*fd);
	free_grabage();
	exit(0);
}

static char	*wait_parent(pid_t pid, int fd, char *file)
{
	int		wstatus;
	int		status;
	t_shell	*shell;

	shell = get_and_set_value(NULL, -1);
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	waitpid(pid, &wstatus, 0);
	close(fd);
	status = WEXITSTATUS(wstatus);
	if (status == 130 || status == 1)
	{
		if (status == 130)
			shell->exit_code = 130;
		else
			shell->exit_code = 1;
		return (NULL);
	}
	shell->exit_code = 0;
	return (file);
}

char	*handle_heredoc(char *delimiter)
{
	char		*filename;
	pid_t		pid;
	int			fd;

	if (prepare_file(&filename, &fd))
		return (NULL);
	pid = fork();
	if (pid < 0)
		return (perror("fork"), NULL);
	if (pid == 0)
		heredoc_child(&fd, delimiter);
	else
		filename = wait_parent(pid, fd, filename);
	return (filename);
}
