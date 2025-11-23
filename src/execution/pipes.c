/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtawil <mtawil@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 02:46:10 by mtawil            #+#    #+#             */
/*   Updated: 2025/11/24 00:18:40 by mtawil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	execute_pipeline(char ***cmds, t_env_and_exit *shell)
{
	int num_cmds;
	int i;
	int **pipes;
	pid_t *pids;
	char *path;
	int status;
	t_cmd *cmd;

	num_cmds = 0;
	while (cmds[num_cmds])
		num_cmds++;

	if (num_cmds == 0)
		return ;

	pipes = malloc(sizeof(int *) * (num_cmds - 1));
	if (!pipes)
		return ;

	i = 0;
	while (i < num_cmds - 1)
	{
		pipes[i] = malloc(sizeof(int) * 2);
		if (!pipes[i] || pipe(pipes[i]) == -1)
		{
			perror("pipe");
			while (--i >= 0)
				free(pipes[i]);
			free(pipes);
			return ;
		}
		i++;
	}

	pids = malloc(sizeof(pid_t) * num_cmds);
	if (!pids)
	{
		i = 0;
		while (i < num_cmds - 1)
		{
			close(pipes[i][0]);
			close(pipes[i][1]);
			free(pipes[i]);
			i++;
		}
		free(pipes);
		return ;
	}

	i = 0;
	while (i < num_cmds)
	{
		cmd = parse_cmd_with_redir(cmds[i]);
		if (!cmd)
		{
			i++;
			continue ;
		}


		path = find_command_path(cmd->args[0], shell);
		if (!path)
		{
			printf("minishell> %s: command not found\n", cmd->args[0]);
			free_cmd(cmd);
			i++;
			continue ;
		}

		pids[i] = fork();
		if (pids[i] == -1)
		{
			perror("fork");
			free(path);
			free_cmd(cmd);
			break ;
		}

		if (pids[i] == 0)
		{
		    reset_signals();
			if (i > 0)
				dup2(pipes[i - 1][0], STDIN_FILENO);

			if (i < num_cmds - 1)
				dup2(pipes[i][1], STDOUT_FILENO);

			int j = 0;
			while (j < num_cmds - 1)
			{
				close(pipes[j][0]);
				close(pipes[j][1]);
				j++;
			}

			if (cmd->redirs)
			{
				if (execute_redirections(cmd->redirs) == -1)
					exit(1);
			}

			execve(path, cmd->args, shell->env);
			perror("minishell: ");
			exit(1);
		}

		free(path);
		free_cmd(cmd);
		i++;
	}

	i = 0;
	while (i < num_cmds - 1)
	{
		close(pipes[i][0]);
		close(pipes[i][1]);
		i++;
	}

	i = 0;
	while (i < num_cmds)
	{
		waitpid(pids[i], &status, 0);
		i++;
	}

	i = 0;
	while (i < num_cmds - 1)
	{
		free(pipes[i]);
		i++;
	}
	free(pipes);
	free(pids);
}