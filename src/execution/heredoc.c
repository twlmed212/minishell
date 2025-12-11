/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtawil <mtawil@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 15:34:17 by mtawil            #+#    #+#             */
/*   Updated: 2025/12/11 18:32:18 by mtawil           ###   ########.fr       */
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
		if (write_to_file(line, delimiter, *fd))
			break ;
	}
	close(*fd);
	free_array(to_fere->shell->env);
	free(to_fere->filename);
	free_cmds(to_fere->cmds);
	exit(0);
}

static char	*wait_parent(pid_t pid, int fd, char *file)
{
	int	wstatus;
	int status;
	t_shell *shell;

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
	else 
		filename =  wait_parent(pid, fd, filename);
	return (filename);
}
