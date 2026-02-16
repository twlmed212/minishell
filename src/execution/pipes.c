/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtawil <mtawil@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 02:46:10 by mtawil            #+#    #+#             */
/*   Updated: 2026/02/16 09:45:10 by mtawil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_cmds(t_cmd *cmds)
{
	int	count;

	count = 0;
	while (cmds)
	{
		count++;
		cmds = cmds->next;
	}
	return (count);
}

void	setup_pipes(int **pipes, int i, int n)
{
	if (i > 0)
		dup2(pipes[i - 1][0], STDIN_FILENO);
	if (i < n - 1)
		dup2(pipes[i][1], STDOUT_FILENO);
}

void	close_pipes(int **pipes, int n)
{
	int	i;

	i = 0;
	while (i < n - 1)
	{
		close(pipes[i][0]);
		close(pipes[i][1]);
		i++;
	}
}

pid_t	exec_pipeline(t_cmd *cmds, t_shell *shell, int **pipes, int n)
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
				return (free_grabage(), exit(1), pid);
			if (!cmds->args || !cmds->args[0])
				return (free_grabage(), exit(0), pid);
			exec_cmd(cmds, shell, shell->env);
		}
		cmds = cmds->next;
		i++;
	}
	close_pipes(pipes, n);
	return (pid);
}

int	**create_pipes(int n)
{
	int	**pipes;
	int	i;

	pipes = ft_malloc(sizeof(int *) * (n - 1));
	i = 0;
	while (i < n - 1)
	{
		pipes[i] = ft_malloc(sizeof(int) * 2);
		if (pipe(pipes[i]) < 0)
			return (perror("pipe"), NULL);
		i++;
	}
	return (pipes);
}
