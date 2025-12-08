/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtawil <mtawil@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 15:34:17 by mtawil            #+#    #+#             */
/*   Updated: 2025/12/08 12:20:15 by mtawil           ###   ########.fr       */
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

static int	prepare_file(char **filename, int *fd)
{
	*filename = generate_unique_tempfile();
	if (!*filename)
		return (1);
	*fd = open(*filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (*fd == -1)
		return (perror("heredoc"), free(*filename), 1);
	return (0);
}

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

char	*handle_heredoc(char *delimiter)
{
	char		*filename;
	pid_t		pid;
	int			fd;
	int			status;
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
	signal(SIGINT, SIG_IGN);
	waitpid(pid, &status, 0);
	setup_signals();
	if (WIFEXITED(status))
		to_free.shell->exit_code = WEXITSTATUS(status);
	close(fd);
	return (filename);
}
