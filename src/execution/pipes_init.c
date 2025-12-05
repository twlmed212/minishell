/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_init.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtawil <mtawil@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/03 17:59:24 by mtawil            #+#    #+#             */
/*   Updated: 2025/12/05 12:12:25 by mtawil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	count_cmds(char ****cmds)
{
	int	num_cmds;

	num_cmds = 0;
	while ((*cmds)[num_cmds])
		num_cmds++;
	return (num_cmds);
}

static int	process_single_heredoc(char ***cmds, int i, int j)
{
	char	*temp_file;

	if (ft_strcmp(cmds[i][j], "<<") == 0 && cmds[i][j + 1])
	{
		temp_file = read_heredoc(cmds[i][j + 1]);
		reset_signals();
		if (!temp_file)
			return (-1);
		cmds[i][j + 1] = temp_file;
	}
	return (0);
}

static int	process_all_heredocs(char ***cmds, int num_cmds)
{
	int		i;
	int		j;

	i = 0;
	while (i < num_cmds)
	{
		j = 0;
		while (cmds[i][j])
		{
			if (process_single_heredoc(cmds, i, j) == -1)
				return (-1);
			j++;
		}
		i++;
	}
	return (0);
}

int	init_pipeline(t_pipeline_data *data, char ***cmds,
		t_env_and_exit *shell)
{
	data->num_cmds = count_cmds(&cmds);
	data->shell = shell;
	if (data->num_cmds == 0)
		return (-1);
	if (process_all_heredocs(cmds, data->num_cmds) == -1)
	{
		if (g_signal == SIGINT)
		{
			shell->last_exit = 130;
			g_signal = 0;
		}
		else
			shell->last_exit = 1;
		return (-1);
	}
	data->pipes = create_pipes(data->num_cmds);
	if (!data->pipes)
		return (-1);
	data->pids = alloc_pids(data->num_cmds, data->pipes);
	if (!data->pids)
		return (-1);
	return (0);
}
