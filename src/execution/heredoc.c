/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtawil <mtawil@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 15:34:17 by mtawil            #+#    #+#             */
/*   Updated: 2025/11/29 14:38:48 by mtawil           ###   ########.fr       */
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

char	*read_heredoc(char *delimiter)
{
	char	*input;
	char	*filename;
	int		fd;

	filename = generate_unique_tempfile();
	if (!filename)
		return (NULL);
	
	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		perror("heredoc");
		free(filename);
		return (NULL);
	}
	
	while (1)
	{
		input = readline("> ");
		
		// CRITICAL FIX: Handle Ctrl+D (NULL input)
		// Ctrl+D should just end THIS heredoc, not be treated as an error
		if (!input)
		{
			// Don't print newline here - bash doesn't
			break ;
		}
		
		// Check if we got the delimiter
		if (ft_strcmp(input, delimiter) == 0)
		{
			free(input);
			break ;
		}
		
		// Write the line to temp file
		write(fd, input, ft_strlen(input));
		write(fd, "\n", 1);
		free(input);
	}
	
	close(fd);
	return (filename);
}
