/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtawil <mtawil@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 02:46:07 by mtawil            #+#    #+#             */
/*   Updated: 2026/02/15 17:42:54 by mtawil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

static void	exec_redir_only(t_cmd *cmd, t_shell *shell)
{
	pid_t	pid;

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
		free_grabage();
		exit(0);
	}
	handle_parent(pid, shell);
}

static void	exec_single(t_cmd *cmd, t_shell *shell)
{
	pid_t	pid;

	if (!cmd->args || !cmd->args[0])
	{
		exec_redir_only(cmd, shell);
		return ;
	}
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

static void	wait_all_process(int n, t_shell *shell, pid_t last_pid)
{
	int		status;
	pid_t	pid;
	int		newline;

	newline = 0;
	while (n--)
	{
		pid = wait(&status);
		if (pid == last_pid)
		{
			if (WIFEXITED(status))
				shell->exit_code = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
			{
				shell->exit_code = 128 + WTERMSIG(status);
				if (newline == 0)
					ft_perror("\n");
				newline = 1;
			}
		}
	}
}

void	executor(t_cmd *cmds, t_shell *shell)
{
	int		**pipes;
	int		n;
	int		i;
	pid_t	last_pid;

	if (!cmds)
		return ;
	if (!cmds->args && !cmds->redirs)
		return ;
	if (cmds->args && !cmds->args[0] && !cmds->redirs)
		return ;
	n = count_cmds(cmds);
	if (n == 1)
		return (exec_single(cmds, shell));
	pipes = create_pipes(n);
	if (!pipes)
		return ;
	last_pid = exec_pipeline(cmds, shell, pipes, n);
	i = 0;
	while (i < n - 1)
		free(pipes[i++]);
	free(pipes);
	disable_parent_signals();
	wait_all_process(n, shell, last_pid);
	setup_signals();
}