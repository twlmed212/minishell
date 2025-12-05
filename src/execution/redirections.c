/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtawil <mtawil@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 02:46:14 by mtawil            #+#    #+#             */
/*   Updated: 2025/12/05 12:18:07 by mtawil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	handledupclose(int fd, char *filename, int unlink_flag, int is_temp)
{
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		perror("dup2");
		close(fd);
		if (unlink_flag && is_temp)
		{
			unlink(filename);
			free(filename);
		}
		return (-1);
	}
	close(fd);
	if (unlink_flag)
	{
		unlink(filename);
		if (is_temp)
			free(filename);
	}
	return (0);
}

static int	open_and_redirect(char *filename, int unlink_flag, int is_temp)
{
	int	fd;

	fd = open(filename, O_RDONLY);
	if (fd == -1)
	{
		perror(filename);
		if (unlink_flag && is_temp)
		{
			unlink(filename);
			free(filename);
		}
		return (-1);
	}
	return (handledupclose(fd, filename, unlink_flag, is_temp));
}

static int	handle_heredoc(t_redir *r)
{
	char	*filename;
	int		unlink_flag;
	int		is_temp;

	unlink_flag = 1;
	if (ft_strncmp(r->file, "/tmp/.heredoc_temp_", 19) == 0)
	{
		filename = r->file;
		is_temp = 0;
	}
	else
	{
		filename = read_heredoc(r->file);
		if (!filename)
			return (ft_perror("minishell: heredoc: interrupted\n"), -1);
		is_temp = 1;
	}
	return (open_and_redirect(filename, unlink_flag, is_temp));
}

int	execute_input_redir(t_redir *r)
{
	int	fd;

	if (r->type == REDIR_HEREDOC)
		return (handle_heredoc(r));
	fd = open(r->file, O_RDONLY);
	if (fd == -1)
		return (perror(r->file), -1);
	if (dup2(fd, STDIN_FILENO) == -1)
		return (perror("dup2"), close(fd), -1);
	close(fd);
	return (0);
}

int	execute_redirections(t_redir *redirs)
{
	t_redir	*current;
	t_env_and_exit *shell;
	current = redirs;
	while (current)
	{

		if (current->type == REDIR_OUT || current->type == REDIR_APPEND)
		{
			if (execute_output_redir(current) == -1)
				return (-1);
		}
		shell = get_and_set_value(NULL, -1);
		if (shell->last_exit == 130)
			break;
		else if (current->type == REDIR_IN || current->type == REDIR_HEREDOC)
		{
			if (execute_input_redir(current) == -1)
				return (-1);
		}
		current = current->next;
	}
	return (0);
}
