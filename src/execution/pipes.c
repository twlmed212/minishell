// /* ************************************************************************** */
// /*                                                                            */
// /*                                                        :::      ::::::::   */
// /*   pipes.c                                            :+:      :+:    :+:   */
// /*                                                    +:+ +:+         +:+     */
// /*   By: mtawil <mtawil@student.1337.ma>            +#+  +:+       +#+        */
// /*                                                +#+#+#+#+#+   +#+           */
// /*   Created: 2025/11/16 02:46:10 by mtawil            #+#    #+#             */
// /*   Updated: 2025/11/27 13:42:50 by mtawil           ###   ########.fr       */
// /*                                                                            */
// /* ************************************************************************** */

#include "../../include/minishell.h"

// Helper function to use cat when there's only redirection
static char *get_default_command(t_cmd *cmd)
{
	// If we have only redirections and no command, use cat
	if (!cmd->args[0] && cmd->redirs)
	{
		// Create a new args array with "cat" as the command
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

void	execute_pipeline(char ***cmds, t_env_and_exit *shell)
{
	int num_cmds;
	int i;
	int **pipes;
	pid_t *pids;
	char *path;
	t_cmd *cmd;
	int should_free_args;

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
		should_free_args = 0;
		cmd = parse_cmd_with_redir(cmds[i]);
		if (!cmd)
		{
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
				should_free_args = 1; // We allocated new args, need to free them
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
            // is builtind Double Lines 
            // convert into function
            // TODO
			printf("here\n");
			int *saved_fds;
            if (is_builtin(cmd->args[0]))
            {
			printf("builtin\n");
                
                saved_fds = save_std_fds();

                if (cmd->redirs)
                {
                    if (execute_redirections(cmd->redirs) == -1)
                    {
                        restore_std_fds(saved_fds);
                        free_cmd(cmd);
                        shell->last_exit = 1;
                    }
                }

                shell->last_exit = run_builtin(cmd->args, shell);

                restore_std_fds(saved_fds);
                free_cmd(cmd);
                i++;
                continue;
            }
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
					// Child process - just exit with 127
					exit(127);
				}
				i++;
				continue;
			}
		}

		pids[i] = fork();
		if (pids[i] == -1)
		{
			perror("fork");
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

			execve(path, cmd->args, shell->env);
			perror("minishell");
			exit(1);
		}

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