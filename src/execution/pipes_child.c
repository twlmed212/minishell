/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_child.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtawil <mtawil@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 18:08:39 by mtawil            #+#    #+#             */
/*   Updated: 2025/12/05 12:07:31 by mtawil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	setup_pipe_redirection(int i, int num_cmds, int **pipes)
{
	if (i > 0)
		dup2(pipes[i - 1][0], STDIN_FILENO);
	if (i < num_cmds - 1)
		dup2(pipes[i][1], STDOUT_FILENO);
}

static void	child_execute_builtin(t_cmd *cmd, t_env_and_exit *shell)
{
	int	exit_code;

	exit_code = run_builtin(cmd->args, shell);
	exit(exit_code);
}

static void	child_execute_command(t_cmd *cmd, char *path,
		t_env_and_exit *shell)
{
	execve(path, cmd->args, shell->env);
	perror("minishell");
	exit(1);
}

static void	child_process(t_cmd *cmd, char *path, t_pipeline_data *data)
{
	reset_signals();
	setup_pipe_redirection(data->i, data->num_cmds, data->pipes);
	close_all_pipes(data->pipes, data->num_cmds);
	if (cmd->redirs)
	{
		if (execute_redirections(cmd->redirs) == -1)
			exit(1);
	}
	if (data->is_builtin)
		child_execute_builtin(cmd, data->shell);
	child_execute_command(cmd, path, data->shell);
}

int	process_single_command(t_pipeline_data *data, char ***cmds)
{
	t_cmd	*cmd;
	char	*path;
	int		builtin_flag;

	builtin_flag = prepare_command(cmds[data->i], data->shell, &cmd, &path);
	if (builtin_flag == -1)
		return (0);
	if (builtin_flag == 0 && !path)
		return (handle_cmd_not_found(cmd, data->i, data->pids));
	data->is_builtin = builtin_flag;
	data->pids[data->i] = fork();
	if (data->pids[data->i] == -1)
		return (handle_fork_error(cmd, path, data), -1);
	if (data->pids[data->i] == 0)
		child_process(cmd, path, data);
	if (path)
		free(path);
	free_cmd(cmd);
	return (0);
}
