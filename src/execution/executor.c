/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtawil <mtawil@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 02:46:07 by mtawil            #+#    #+#             */
/*   Updated: 2025/12/07 22:18:46 by mtawil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	exec_single(t_cmd *cmd, t_shell *shell)
{
	pid_t	pid;
	int		status;

	if (is_builtin(cmd->args[0]) && !cmd->next && !cmd->redirs)
	{
		shell->exit_code = exec_builtin(cmd, shell);
		return ;
	}
	pid = fork();
	if (pid < 0)
	{
		perror("fork");
		return ;
	}
	if (pid == 0)
	{
		if (handle_redirs(cmd->redirs) < 0)
			exit(1);
		exec_cmd(cmd, shell, shell->env);
	}
	init_signals_child_exec();
	waitpid(pid, &status, 0);
	setup_signals();
	if (WIFEXITED(status))
		shell->exit_code = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		shell->exit_code = 128 + WTERMSIG(status);
	if (shell->exit_code == 130)
		write(1, "\n", 1);
}

static void	exec_pipeline(t_cmd *cmds, t_shell *shell, int **pipes, int n)
{
	pid_t	pid;
	int		i;

	i = 0;
	while (i < n)
	{
		pid = fork();
		if (pid == 0)
		{
			setup_pipes(pipes, i, n);
			close_pipes(pipes, n);
			if (handle_redirs(cmds->redirs) < 0)
				exit(1);
			exec_cmd(cmds, shell, shell->env);
		}
		cmds = cmds->next;
		i++;
	}
	close_pipes(pipes, n);
}

void	executor(t_cmd *cmds, t_shell *shell)
{
	int **pipes;
	int n;
	int i;
	int status;

	if ((!cmds || !cmds->args[0]))
		return ;
	n = count_cmds(cmds);
	if (n == 1)
		return (exec_single(cmds, shell));
	pipes = create_pipes(n);
	if (!pipes)
		return ;
	exec_pipeline(cmds, shell, pipes, n);
	i = 0;
	while (i < n - 1)
		free(pipes[i++]);
	free(pipes);
	while (n--)
	{
		wait(&status);
		if (WIFEXITED(status))
			shell->exit_code = WEXITSTATUS(status);
	}
}