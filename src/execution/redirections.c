/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtawil <mtawil@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 02:46:14 by mtawil            #+#    #+#             */
/*   Updated: 2025/12/01 20:44:22 by mtawil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	*save_std_fds(void)
{
	int	*saved;

	saved = malloc(sizeof(int) * 2);
	if (!saved)
		return (NULL);
	saved[0] = dup(STDIN_FILENO);
	saved[1] = dup(STDOUT_FILENO);
	return (saved);
}

void	restore_std_fds(int *saved)
{
	if (!saved)
		return ;
	dup2(saved[0], STDIN_FILENO);
	dup2(saved[1], STDOUT_FILENO);
	close(saved[0]);
	close(saved[1]);
	free(saved);
}

int	execute_output_redir(t_redir *redir)
{
	int	fd;
	int	flags;

	if (redir->type == REDIR_OUT)
		flags = O_WRONLY | O_CREAT | O_TRUNC;
	else
		flags = O_WRONLY | O_CREAT | O_APPEND;
	fd = open(redir->file, flags, 0644);
	if (fd == -1)
	{
		perror(redir->file);
		return (-1);
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		perror("dup2");
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}

int	execute_input_redir(t_redir *redir)
{
	int		fd;
	char	*filename;
	int		should_unlink;

	should_unlink = 0;
	if (redir->type == REDIR_HEREDOC)
	{
		if (ft_strncmp(redir->file, "/tmp/.heredoc_temp_", 19) == 0)
		{
			filename = redir->file;
			should_unlink = 1;
		}
		else
		{
			filename = read_heredoc(redir->file);
			if (!filename)
			{
				ft_perror("minishell: heredoc: interrupted\n");
				return (-1);
			}
			should_unlink = 1;
		}
		fd = open(filename, O_RDONLY);
		if (fd == -1)
		{
			perror(filename);
			if (should_unlink && filename != redir->file)
			{
				unlink(filename);
				free(filename);
			}
			return (-1);
		}
		if (dup2(fd, STDIN_FILENO) == -1)
		{
			perror("dup2");
			close(fd);
			if (should_unlink && filename != redir->file)
			{
				unlink(filename);
				free(filename);
			}
			return (-1);
		}
		close(fd);
		if (should_unlink)
		{
			unlink(filename);
			if (filename != redir->file)
				free(filename);
		}
	}
	else
	{
		fd = open(redir->file, O_RDONLY);
		if (fd == -1)
		{
			perror(redir->file);
			return (-1);
		}
		if (dup2(fd, STDIN_FILENO) == -1)
		{
			perror("dup2");
			close(fd);
			return (-1);
		}
		close(fd);
	}
	return (0);
}

int	execute_redirections(t_redir *redirs)
{
	t_redir	*current;

	current = redirs;
	while (current)
	{
		if (current->type == REDIR_OUT || current->type == REDIR_APPEND)
		{
			if (execute_output_redir(current) == -1)
				return (-1);
		}
		else if (current->type == REDIR_IN || current->type == REDIR_HEREDOC)
		{
			if (execute_input_redir(current) == -1)
				return (-1);
		}
		current = current->next;
	}
	return (0);
}
