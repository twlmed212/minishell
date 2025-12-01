/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtawil <mtawil@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 15:34:17 by mtawil            #+#    #+#             */
/*   Updated: 2025/12/01 15:20:35 by mtawil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// CRITICAL FIX: Generate unique temp file for each heredoc
static char *generate_unique_tempfile(void)
{
	static int counter = 0;
	char *num_str;
	char *temp1;
	char *filename;
	
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
static int write_to_file(char *input, char *delimiter, int fd)
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
static int prepare_file(char **filename, int *fd)
{
	*filename = generate_unique_tempfile();
	if (!*filename)
		return (1);
	*fd = open(*filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (*fd == -1)
		return (perror("heredoc"), free(*filename), 1);
	return (0);
}

char	*read_heredoc(char *delimiter)
{
	char	*input;
	char	*filename;
	int		fd;

	if (prepare_file( &filename, &fd))
		return (NULL);
	while (1)
	{
		input = readline("> ");
		if (g_signal == SIGINT)
		{
			if (input)
				free(input);
			close(fd);
			unlink(filename);
			free(filename);
			g_signal = 0;
			return (NULL);
		}
		if (write_to_file(input, delimiter, fd))
			break ;
		
	}
	close(fd);
	return (filename);
}
