/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtawil <mtawil@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 02:46:07 by mtawil            #+#    #+#             */
/*   Updated: 2025/12/17 23:43:55 by mtawil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	handle_parent(pid_t pid, t_shell *shell)
{
	int	status;

	disable_parent_signals();
	waitpid(pid, &status, 0);
	setup_signals();
	if (WIFEXITED(status))
		shell->exit_code = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		shell->exit_code = 128 + WTERMSIG(status);
	if (shell->exit_code >= 130)
		write(1, "\n", 1);
}

static void	exec_single(t_cmd *cmd, t_shell *shell)
{
	pid_t	pid;

	if (is_builtin(cmd->args[0]) && !cmd->next && !cmd->redirs)
	{
		shell->exit_code = exec_builtin(cmd, shell);
		return ;
	}
	pid = fork();
	if (pid < 0)
		return ((void)perror("fork"));
	if (pid == 0)
	{
		if (handle_redirs(cmd->redirs) < 0)
		{
			free_grabage();
			exit(1);
		}
		exec_cmd(cmd, shell, shell->env);
	}
	handle_parent(pid, shell);
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
			{
				free_pipes(pipes, n, 1);
				exit(1);
			}
			free_pipes(pipes, n, 0);
			exec_cmd(cmds, shell, shell->env);
		}
		cmds = cmds->next;
		i++;
	}
	close_pipes(pipes, n);
}

static void	wait_all_process(int n, t_shell *shell)
{
	int	status;
	int	newline;

	newline = 0;
	while (n--)
	{
		wait(&status);
		if (WIFEXITED(status))
			shell->exit_code = WEXITSTATUS(status);
		if (WIFSIGNALED(status))
		{
			shell->exit_code = 128 + WTERMSIG(status);
			if (newline == 0)
				ft_perror("\n");
			newline = 1;
		}
	}
}

void	executor(t_cmd *cmds, t_shell *shell)
{
	int	**pipes;
	int	n;
	int	i;

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
	disable_parent_signals();
	wait_all_process(n, shell);
	setup_signals();
}
