/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtawil <mtawil@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 16:45:53 by mtawil            #+#    #+#             */
/*   Updated: 2026/02/16 10:34:25 by mtawil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_errors(char *cmnd, int type)
{
	ft_perror("minishell: ");
	ft_perror(cmnd);
	if (type)
		ft_perror(" command not found\n");
	else
		ft_perror(" No such file or directory\n");
	free_grabage();
	exit(127);
}

void	exec_cmd(t_cmd *cmd, t_shell *shell, char **env)
{
	char	*path;

	restore_signals();
	if (is_builtin(cmd->args[0]))
	{
		shell->exit_code = exec_builtin(cmd, shell);
		free_grabage();
		exit(shell->exit_code);
	}
	if (ft_strlen(cmd->args[0]) == 0)
		handle_errors(cmd->args[0], 1);
	path = find_path(cmd->args[0], env);
	if (!path)
		handle_errors(cmd->args[0], 1);
	execve(path, cmd->args, env);
	if (path)
		handle_errors(path, 0);
	perror("execve");
	free_grabage();
	exit(1);
}
