/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtawil <mtawil@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 02:46:10 by mtawil            #+#    #+#             */
/*   Updated: 2025/12/05 16:58:31 by mtawil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static char	*get_command_path(t_cmd *cmd, t_env_and_exit *shell,
		int *is_built)
{
	char	*path;

	if (is_builtin(cmd->args[0]))
	{
		*is_built = 1;
		return (NULL);
	}
	path = find_command_path(cmd->args[0], shell);
	return (path);
}

int	prepare_command(char **cmd_args, t_env_and_exit *shell,
		t_cmd **cmd, char **path)
{
	int	builtin_flag;

	builtin_flag = 0;
	*cmd = parse_cmd_with_redir(cmd_args);
	if (!*cmd || ((*cmd)->args && !(*cmd)->args[0]))
	{
		if (*cmd)
			free_cmd(*cmd);
		return (-1);
	}
	*path = get_command_path(*cmd, shell, &builtin_flag);
	if (!*path && !builtin_flag)
		return (0);
	return (builtin_flag);
}

static void	wait_all_children(pid_t *pids, int num_cmds)
{
	int	i;
	int	status;

	i = 0;
	while (i < num_cmds)
	{
		waitpid(pids[i], &status, 0);
		if (WIFEXITED(status))
		{
			status = WEXITSTATUS(status);
			if (status == 130 || status == 1)
			{
				if (status == 130)
					get_and_set_value(NULL, 130);
				else
					get_and_set_value(NULL, 1);
				return;
			}
		}
		i++;
	}
}

void	execute_pipeline(char ***cmds, t_env_and_exit *shell)
{
	t_pipeline_data	data;

	if (init_pipeline(&data, cmds, shell) == -1)
		return ;
	data.i = 0;
	while (data.i < data.num_cmds)
	{
		if (process_single_command(&data, cmds) == -1)
			return ;
		data.i++;
	}
	close_all_pipes(data.pipes, data.num_cmds);
	wait_all_children(data.pids, data.num_cmds);
	free_pipes_array(data.pipes, data.num_cmds);
	free(data.pids);
}
