/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtawil <mtawil@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 14:24:14 by mtawil            #+#    #+#             */
/*   Updated: 2025/12/07 16:48:21 by mtawil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	is_var_valid(char c, int is_first)
{
	if (is_first)
		return (ft_isalpha(c) || c == '_');
	return (ft_isalnum(c) || c == '_');
}

static int	is_name_valid(char *name)
{
	int	i;

	if (!name || !name[0])
		return (0);
	if (!is_var_valid(name[0], 1))
		return (0);
	i = 1;
	while (name[i])
	{
		if (!is_var_valid(name[i], 0))
			return (0);
		i++;
	}
	return (1);
}

static int	p_err(char *arg)
{
	ft_perror("minishell: export: `");
	ft_perror(arg);
	ft_perror("': not a valid identifier\n");
	return (1);
}
char	*get_name(char *arg, t_shell *shell, int *ret, int *i)
{
	char	*name;
	char	*equal_sign;

	if (arg[0] == '=')
		return (p_err(arg), (*ret)++, (*i)++, NULL);
	equal_sign = ft_strchr(arg, '=');
	if (!equal_sign)
	{
		if (!is_name_valid(arg))
			return (p_err(arg), (*ret)++, (*i)++, NULL);
		shell->running = set_env(arg, "", shell);
		return ((*ret)++, (*i)++, NULL);
	}
	name = ft_substr(arg, 0, equal_sign - arg);
	if (!is_name_valid(name))
	{
		p_err(arg);
		return (free(name), (*ret)++, (*i)++, NULL);
	}
	return (name);
}

char	*get_value(char *arg, char *name, int *ret, int *i)
{
	char	*value;
	char	*clean_value;

	value = ft_strdup(arg + ft_strlen(name) + 1);
	if (!value)
	{
		free(name);
		return ((*ret)++, (*i)++, NULL);
	}
	clean_value = remove_quotes(value);
	free(value);
	if (!clean_value)
	{
		free(name);
		return ((*ret)++, (*i)++, NULL);
	}
	return (clean_value);
}
