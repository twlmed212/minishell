/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_builtin.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtawil <mtawil@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/30 14:21:59 by mtawil            #+#    #+#             */
/*   Updated: 2025/12/05 12:07:40 by mtawil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	execute_builtin_cmd(t_cmd *cmd, char **args, t_env_and_exit *shell)
{
	int	*saved_fds;

	if (ft_strcmp(cmd->args[0], "exit") == 0)
		return (free_cmd(cmd), builtin_exit(args, shell), 1);
	saved_fds = save_std_fds();
	if (cmd->redirs)
	{
		if (execute_redirections(cmd->redirs) == -1)
		{
			restore_std_fds(saved_fds);
			free_cmd(cmd);
			free_array(args);
			shell->last_exit = 1;
			return (1);
		}
	}
	shell->last_exit = run_builtin(cmd->args, shell);
	restore_std_fds(saved_fds);
	free_cmd(cmd);
	free_array(args);
	return (1);
}
