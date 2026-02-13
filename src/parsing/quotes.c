/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtawil <mtawil@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 02:46:22 by mtawil            #+#    #+#             */
/*   Updated: 2026/02/13 00:11:29 by mtawil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	length_without_qoutes(char *str)
{
	int		len;
	int		i;
	char	qoute_found;

	len = 0;
	i = -1;
	qoute_found = 0;
	while (str[++i])
	{
		if (qoute_found == 0)
		{
			if (str[i] == '\'' || str[i] == '"')
				qoute_found = str[i];
			else
				len++;
		}
		else
		{
			if (str[i] == qoute_found)
				qoute_found = 0;
			else
				len++;
		}
	}
	return (len);
}

static void	copy_without_qoutes(char *str, char *result)
{
	int		i;
	int		j;
	char	qoute_found;

	i = -1;
	j = 0;
	qoute_found = 0;
	while (str[++i])
	{
		if (qoute_found == 0)
		{
			if (str[i] == '\'' || str[i] == '"')
				qoute_found = str[i];
			else
				result[j++] = str[i];
		}
		else
		{
			if (str[i] == qoute_found)
				qoute_found = 0;
			else
				result[j++] = str[i];
		}
	}
	result[j] = '\0';
}

char	*remove_qoutes(char *str)
{
	char	*result;
	int		len;

	if (!str)
		return (NULL);
	len = length_without_qoutes(str);
	result = ft_malloc(sizeof(char) * (len + 1));
	if (!result)
		return (NULL);
	copy_without_qoutes(str, result);
	return (result);
}

int	check_unclosed_qoutes(char *line)
{
	int		i;
	char	qoutes;

	i = 0;
	while (line[i])
	{
		if (line[i] == '\'' || line[i] == '"')
		{
			qoutes = line[i];
			i++;
			while (line[i] && line[i] != qoutes)
				i++;
			if (!line[i])
			{
				ft_perror("minishell: error: unclosed quotes\n");
				return (1);
			}
		}
		i++;
	}
	return (0);
}