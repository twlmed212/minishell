/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_set.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtawil <mtawil@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 02:46:28 by mtawil            #+#    #+#             */
/*   Updated: 2026/02/14 13:33:38 by mtawil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*update_the_value(char *temp, char *value, t_shell *shell,
		char *name)
{
	char	*new_var;
	int		i;

	new_var = ft_strjoin(temp, value);
	if (!new_var)
	{
		shell->running = 1;
		return (NULL);
	}
	i = 0;
	while (shell->env[i])
	{
		if ((ft_strncmp(shell->env[i], name, ft_strlen(name)) == 0)
			&& (shell->env[i][ft_strlen(name)] == '='
			|| shell->env[i][ft_strlen(name)] == '\0'))
		{
			shell->env[i] = new_var;
			shell->running = 0;
			return (NULL);
		}
		i++;
	}
	return (new_var);
}

static char	**re_create_env(t_shell *shell, char *new_var)
{
	int		count;
	char	**new_env;
	int		i;

	count = 0;
	while (shell->env[count])
		count++;
	new_env = ft_malloc(sizeof(char *) * (count + 2));
	if (!new_env)
		return (NULL);
	i = 0;
	while (i < count)
	{
		new_env[i] = shell->env[i];
		i++;
	}
	new_env[i] = new_var;
	new_env[i + 1] = NULL;
	return (new_env);
}

int	set_env(char *name, char *value, t_shell *shell)
{
	char	*new_var;
	char	*temp;
	char	**new_env;

	if (value && value[0] != '\0')
		temp = ft_strjoin(name, "=");
	else
		temp = ft_strdup(name);
	if (!temp)
		return (1);
	new_var = update_the_value(temp, value, shell, name);
	if (!new_var)
		return (0);
	new_env = re_create_env(shell, new_var);
	if (!new_env)
		return (1);
	shell->env = new_env;
	return (0);
}
