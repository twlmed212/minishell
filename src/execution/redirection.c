/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtawil <mtawil@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/16 09:39:17 by mtawil            #+#    #+#             */
/*   Updated: 2026/02/16 09:39:29 by mtawil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	execute_output_redir(t_redir *redir)
{
	int	fd;
	int	flags;

	if (redir->type == T_REDIR_OUT)
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

static int	execute_input_redir(t_redir *r)
{
	int	fd;

	fd = open(r->file, O_RDONLY);
	if (fd == -1)
	{
		perror(r->file);
		return (-1);
	}
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		perror("dup2");
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}

int	handle_redirs(t_redir *redirs)
{
	while (redirs)
	{
		if (redirs->type == T_REDIR_OUT || redirs->type == T_APPEND)
		{
			if (execute_output_redir(redirs) == -1)
				return (-1);
		}
		else if (redirs->type == T_REDIR_IN || redirs->type == T_HEREDOC)
		{
			if (execute_input_redir(redirs) == -1)
				return (-1);
		}
		redirs = redirs->next;
	}
	return (0);
}
