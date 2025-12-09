/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtawil <mtawil@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 16:45:53 by mtawil            #+#    #+#             */
/*   Updated: 2025/12/08 22:55:26 by mtawil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	exec_cmd(t_cmd *cmd, t_shell *shell, char **env)
{
	char	*path;

	if (is_builtin(cmd->args[0]))
	{
		shell->exit_code = exec_builtin(cmd, shell);
		exit(shell->exit_code);
	}
	path = find_path(cmd->args[0], env);
	if (!path)
	{
		ft_perror(cmd->args[0]);
		ft_perror(": command not found\n");
		free_cmds(cmd);
		free_array(shell->env);
		exit(127);
	}
	execve(path, cmd->args, env);
	perror("execve");
	exit(1);
}

int	execute_output_redir(t_redir *redir)
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

int	execute_input_redir(t_redir *r)
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
