/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_pipe.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtawil <mtawil@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/22 03:24:37 by mtawil            #+#    #+#             */
/*   Updated: 2025/11/22 06:12:36 by mtawil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	count_pipes(char **args)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (args[i])
	{
		if (ft_strcmp(args[i], "|") == 0)
			count++;
		i++;
	}
	return (count);
}

int	has_pipe(char **args)
{
	return (count_pipes(args) > 0);
}

int	find_pipe(char **args)
{
	int	i;

	i = 0;
	while (args[i])
	{
		if (ft_strcmp(args[i], "|") == 0)
			return (i);
		i++;
	}
	return (-1);
}

char	***split_all_pipes(char **args)
{
	char	***result;
	int		num_cmds;
	int		start;

	int i, j, k;
	num_cmds = count_pipes(args) + 1;
	result = malloc(sizeof(char **) * (num_cmds + 1));
	if (!result)
		return (NULL);
	i = 0;
	start = 0;
	k = 0;
	while (i < num_cmds)
	{
		j = start;
		while (args[j] && ft_strcmp(args[j], "|") != 0)
			j++;
		result[i] = malloc(sizeof(char *) * (j - start + 1));
		if (!result[i])
		{
			while (--i >= 0)
				free(result[i]);
			free(result);
			return (NULL);
		}
		k = 0;
		while (start < j)
		{
			result[i][k] = args[start];
			k++;
			start++;
		}
		result[i][k] = NULL;
		if (args[start])
			start++;
		i++;
	}
	result[num_cmds] = NULL;
	return (result);
}

void	free_all_pipes(char ***cmds)
{
	int	i;

	if (!cmds)
		return ;
	i = 0;
	while (cmds[i])
	{
		free(cmds[i]);
		i++;
	}
	free(cmds);
}
