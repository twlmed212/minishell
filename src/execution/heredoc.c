/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtawil <mtawil@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 15:34:17 by mtawil            #+#    #+#             */
/*   Updated: 2025/12/09 15:07:51 by mtawil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	heredoc_child(int *fd, char *delimiter, t_to_free *to_fere)
{
	char	*line;

	signal(SIGINT, handle_sigint_heredoc);
	signal(SIGQUIT, SIG_IGN);
	while (1)
	{
		line = readline("> ");
		if (g_signal == SIGINT)
		{
			if (line)
				free(line);
			g_signal = 0;
			break ;
		}
		if (write_to_file(line, delimiter, *fd))
			break ;
	}
	close(*fd);
	free_array(to_fere->shell->env);
	free(to_fere->filename);
	free_cmds(to_fere->cmds);
	exit(0);
}

static int	wait_parent(pid_t pid, int fd, t_to_free *to_free)
{
	int	status;

	signal(SIGINT, SIG_IGN);
	waitpid(pid, &status, 0);
	setup_signals();
	close(fd);
	if (WIFEXITED(status))
		to_free->shell->exit_code = WEXITSTATUS(status);
	if (WIFSIGNALED(status) && WIFEXITED(status) != 0)
		to_free->shell->exit_code = WTERMSIG(status);
	if (to_free->shell->exit_code >= 128)
		return (-1);
	return (0);
}

char	*handle_heredoc(char *delimiter)
{
	char		*filename;
	pid_t		pid;
	int			fd;
	t_to_free	to_free;

	to_free.shell = get_and_set_value(NULL, -1);
	to_free.cmds = get_pointer_cmds(NULL);
	if (prepare_file(&filename, &fd))
		return (NULL);
	to_free.filename = filename;
	pid = fork();
	if (pid < 0)
		return (perror("fork"), NULL);
	if (pid == 0)
		heredoc_child(&fd, delimiter, &to_free);
	if (wait_parent(pid, fd, &to_free) == -1)
		return (NULL);
	return (filename);
}
