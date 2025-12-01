/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtawil <mtawil@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 02:46:10 by mtawil            #+#    #+#             */
/*   Updated: 2025/12/01 15:27:52 by mtawil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static char *get_default_command(t_cmd *cmd)
{
	if (!cmd->args[0] && cmd->redirs)
	{
		char **new_args = malloc(sizeof(char *) * 2);
		if (!new_args)
			return NULL;
		new_args[0] = ft_strdup("cat");
		new_args[1] = NULL;
		cmd->args = new_args;
		return ft_strdup("cat");
	}
	return NULL;
}

static int process_all_heredocs(char ***cmds, int num_cmds)
{
	int i;
	int j;
	char *temp_file;
	
	i = 0;
	while (i < num_cmds)
	{
		j = 0;
		while (cmds[i][j])
		{
			if (ft_strcmp(cmds[i][j], "<<") == 0 && cmds[i][j + 1])
			{
				temp_file = read_heredoc(cmds[i][j + 1]);
				if (!temp_file)
					return (-1);
				cmds[i][j + 1] = temp_file;
			}
			j++;
		}
		i++;
	}
	return (0);
}

void	execute_pipeline(char ***cmds, t_env_and_exit *shell)
{
	int num_cmds;
	int i;
	int **pipes;
	pid_t *pids;
	char *path;
	t_cmd *cmd;
	int should_free_args;
	int is_builtin_cmd;

	num_cmds = 0;
	while (cmds[num_cmds])
		num_cmds++;

	if (num_cmds == 0)
		return ;

	// CRITICAL FIX: Process ALL heredocs FIRST, modifying the cmds array
	if (process_all_heredocs(cmds, num_cmds) == -1)
	{
		// Set exit status to 130 if Ctrl+C was pressed
		if (g_signal == SIGINT)
		{
			shell->last_exit = 130;
			g_signal = 0;
		}
		else
		{
			shell->last_exit = 1;
		}
		return;
	}

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
		should_free_args = 0;
		is_builtin_cmd = 0;
		cmd = parse_cmd_with_redir(cmds[i]);
		if (!cmd)
		{
			i++;
			continue;
		}
        if (cmd->args && !cmd->args[0])
        {
            free_cmd(cmd);
            i++;
            continue;
        }
		// Handle case where we have only redirections (like "<< EOF |")
		if (cmd->args && !cmd->args[0])
		{
			char *default_cmd = get_default_command(cmd);
			if (default_cmd)
			{
				path = default_cmd;
				should_free_args = 1;
			}
			else
			{
				ft_perror("minishell: syntax error near pipe\n");
				free_cmd(cmd);
				int j = 0;
				while (j < num_cmds - 1)
				{
					close(pipes[j][0]);
					close(pipes[j][1]);
					free(pipes[j]);
					j++;
				}
				free(pipes);
				free(pids);
				return;
			}
		}
		else
		{
			// CRITICAL FIX: Check if it's a builtin
			if (is_builtin(cmd->args[0]))
			{
				// For builtins in pipes, we need to fork so they run in the pipeline
				path = NULL;  // Signal that this is a builtin
				is_builtin_cmd = 1;
			}
			else
			{
				path = find_command_path(cmd->args[0], shell);
				if (!path)
				{
					ft_perror("minishell: ");
					ft_perror(cmd->args[0]);
					ft_perror(": command not found\n");
					free_cmd(cmd);
					pids[i] = fork();
					if (pids[i] == 0)
					{
						exit(127);
					}
					i++;
					continue;
				}
			}
		}

		pids[i] = fork();
		if (pids[i] == -1)
		{
			perror("fork");
			if (path)
				free(path);
			if (should_free_args && cmd->args)
			{
				free_array(cmd->args);
				cmd->args = NULL;
			}
			free_cmd(cmd);
			int j = 0;
			while (j < num_cmds - 1)
			{
				close(pipes[j][0]);
				close(pipes[j][1]);
				free(pipes[j]);
				j++;
			}
			free(pipes);
			free(pids);
			return;
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

			// CRITICAL FIX: Handle builtins in child process
			if (is_builtin_cmd)
			{
				int exit_code = run_builtin(cmd->args, shell);
				exit(exit_code);
			}

			execve(path, cmd->args, shell->env);
			perror("minishell");
			exit(1);
		}

		if (path)
			free(path);
		if (should_free_args && cmd->args)
		{
			free_array(cmd->args);
			cmd->args = NULL;
		}
		free_cmd(cmd);
		i++;
	}

	i = 0;
	while (i < num_cmds - 1)
	{
		close(pipes[i][0]);
		close(pipes[i][1]);
		free(pipes[i]);
		i++;
	}

	i = 0;
	int status;
	while (i < num_cmds)
	{
		waitpid(pids[i], &status, 0);
		if (WIFEXITED(status))
			shell->last_exit = WEXITSTATUS(status);
		else
			shell->last_exit = 1;
		
		i++;
	}

	free(pipes);
	free(pids);
}