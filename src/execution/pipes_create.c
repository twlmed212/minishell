/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_create.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtawil <mtawil@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 18:01:09 by mtawil            #+#    #+#             */
/*   Updated: 2025/12/05 12:31:09 by mtawil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	cleanup_pipes_on_error(int **pipes, int i)
{
	while (--i >= 0)
		free(pipes[i]);
	free(pipes);
}

int	**create_pipes(int num_cmds)
{
	int	**pipes;
	int	i;

	pipes = malloc(sizeof(int *) *120000);
	if (!pipes)
		return (NULL);
	i = 0;
	while (i < num_cmds - 1)
	{
		pipes[i] = malloc(sizeof(int) * 2);
		if (!pipes[i] || pipe(pipes[i]) == -1)
		{
			perror("pipe");
			cleanup_pipes_on_error(pipes, i);
			return (NULL);
		}
		i++;
	}
	return (pipes);
}

void	free_pipes_array(int **pipes, int num_cmds)
{
	int	i;

	i = 0;
	while (i < num_cmds - 1)
	{
		close(pipes[i][0]);
		close(pipes[i][1]);
		free(pipes[i]);
		i++;
	}
	free(pipes);
}

pid_t	*alloc_pids(int num_cmds, int **pipes)
{
	pid_t	*pids;

	pids = malloc(sizeof(pid_t) * num_cmds);
	if (!pids)
	{
		free_pipes_array(pipes, num_cmds);
		return (NULL);
	}
	return (pids);
}
