/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtawil <mtawil@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 02:46:07 by mtawil            #+#    #+#             */
/*   Updated: 2025/11/22 08:07:32 by mtawil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	execute_command(char *command, t_env_and_exit *shell)
{
	pid_t pid;
	char **args;
	char **expanded_args;
	char *cmd_path;
	t_cmd *cmd;
	int *saved_fds;
	int status;

	printf(" command: %s\n", command);
	args = parse_command(command);
	int i = 0;
	while (args && args[i])
	{
		printf(" arg[%d]: %s\n", i, args[i]);
		i++;
	}
	if (!args || !args[0])
	{
		free_array(args);
		return ;
	}

	if (has_pipe(args))
	{
		char ***pipe_cmds = split_all_pipes(args);

		if (pipe_cmds)
		{
			execute_pipeline(pipe_cmds, shell);
			free_all_pipes(pipe_cmds);
		}

		free_array(args);
		return ;
	}

	cmd = parse_cmd_with_redir(args);
	if (!cmd)
	{
		free_array(args);
		return ;
	}
	expanded_args = expand_args(cmd->args, shell);
	if (!expanded_args)
	{
		free_cmd(cmd);
		return ;
	}

	if (is_builtin(expanded_args[0]))
	{
		saved_fds = save_std_fds();

		if (cmd->redirs)
		{
			if (execute_redirections(cmd->redirs) == -1)
			{
				restore_std_fds(saved_fds);
				free_array(expanded_args);
				free_cmd(cmd);
				shell->last_exit = 1;
				return ;
			}
		}

		shell->last_exit = run_builtin(expanded_args, shell);

		restore_std_fds(saved_fds);
		free_array(expanded_args);
		free_cmd(cmd);
		return ;
	}

	cmd_path = find_command_path(expanded_args[0], shell);
	if (!cmd_path)
	{
		printf("minishell: %s: command not found\n", expanded_args[0]);
		free_array(expanded_args);
		free_cmd(cmd);
		shell->last_exit = 127;
		return ;
	}

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		free(cmd_path);
		free_array(expanded_args);
		free_cmd(cmd);
		shell->last_exit = 1;
		return ;
	}

	if (pid == 0)
	{
		if (cmd->redirs)
		{
			if (execute_redirections(cmd->redirs) == -1)
				exit(1);
		}

		if (execve(cmd_path, expanded_args, shell->env) == -1)
		{
			perror("execve");
			exit(126);
		}
	}
	else
	{
		wait(&status);

		if (WIFEXITED(status))
			shell->last_exit = WEXITSTATUS(status);
		else
			shell->last_exit = 1;
	}

	free(cmd_path);
	free_array(expanded_args);
	free_cmd(cmd);
}