/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtawil <mtawil@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 02:46:05 by mtawil            #+#    #+#             */
/*   Updated: 2025/12/17 16:45:46 by mtawil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	get_position(char *name, t_shell *shell)
{
	int	i;
	int	len;

	i = 0;
	len = ft_strlen(name);
	while (shell->env[i])
	{
		if ((ft_strncmp(shell->env[i], name, len) == 0)
			&& (shell->env[i][len] == '=' || shell->env[i][len] == '\0'))
		{
			return (i);
		}
		i++;
	}
	if (!shell->env[i])
		return (0);
	return (0);
}

static int	alloc_new_env(t_shell *shell, char ***new_env)
{
	int	new_size;

	new_size = 0;
	while (shell->env[new_size])
		new_size++;
	*new_env = ft_malloc(sizeof(char *) * new_size);
	if (!*new_env)
		return (1);
	return (0);
}

static void	fill_new_env(t_shell *shell, char ***new_env, int pos)
{
	int	j;
	int	k;

	j = 0;
	k = 0;
	while (shell->env[j])
	{
		if (j != pos)
		{
			(*new_env)[k] = shell->env[j];
			k++;
		}
		else
		{
			free(shell->env[j]);
		}
		j++;
	}
	(*new_env)[k] = NULL;
	free(shell->env);
	shell->env = *new_env;
}

static int	ft_unset(char *name, t_shell *shell)
{
	int		pos;
	char	**new_env;
	int		err;
	char	*str;

	new_env = NULL;
	str = ft_strchr(name, '=');
	if (str)
		return (1);
	pos = get_position(name, shell);
	err = alloc_new_env(shell, &new_env);
	if (err)
		return (1);
	fill_new_env(shell, &new_env, pos);
	return (0);
}

int	builtin_unset(char **args, t_shell *shell)
{
	int	i;

	if (!args[1])
		return (0);
	i = 1;
	while (args[i])
	{
		ft_unset(args[i], shell);
		i++;
	}
	return (0);
}
