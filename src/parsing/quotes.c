/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtawil <mtawil@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 02:46:22 by mtawil            #+#    #+#             */
/*   Updated: 2025/12/17 16:45:46 by mtawil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void		ft_perror(char *s);

static int	is_quote(char c)
{
	return (c == '\'' || c == '"');
}

static void	remove_helper_util(char c, t_quotes *data)
{
	if (is_quote(c) && !data->quote)
	{
		data->quote = c;
		(data->i)++;
	}
	else if (c == data->quote)
	{
		data->quote = 0;
		(data->i)++;
	}
	else
	{
		data->result[data->j] = c;
		(data->j)++;
		(data->i)++;
	}
}

char	*remove_quotes(char *str)
{
	t_quotes	data;

	if (!str)
		return (ft_strdup(""));
	data.result = ft_malloc(ft_strlen(str) + 1);
	if (!data.result)
		return (NULL);
	data.i = 0;
	data.j = 0;
	data.quote = 0;
	while (str[data.i])
	{
		remove_helper_util(str[data.i], &data);
	}
	data.result[data.j] = '\0';
	return (data.result);
}

int	check_unclosed_quotes(char *line)
{
	int		i;
	char	c;

	i = 0;
	while (line[i])
	{
		if (line[i] == '\'' || line[i] == '\"')
		{
			c = line[i];
			i++;
			while (line[i] != c && line[i])
				i++;
			if (!line[i])
			{
				ft_perror("error: unclosed quotes\n");
				free(line);
				return (-1);
			}
			i++;
		}
		else
			i++;
	}
	return (1);
}
