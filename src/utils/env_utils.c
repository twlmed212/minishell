/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtawil <mtawil@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 02:46:28 by mtawil            #+#    #+#             */
/*   Updated: 2025/12/01 23:25:33 by mtawil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	**copy_env(char **envp)
{
	char	**new_env;
	int		i;
	int		count;

	count = 0;
	while (envp[count])
		count++;
	new_env = malloc(sizeof(char *) * (count + 1));
	if (!new_env)
		return (NULL);
	i = 0;
	while (envp[i])
	{
		new_env[i] = ft_strdup(envp[i]);
		if (!new_env[i])
		{
			free_array(new_env);
			return (NULL);
		}
		i++;
	}
	new_env[i] = NULL;
	return (new_env);
}

char	*get_env_value(char *name, t_env_and_exit *shell)
{
	int	i;
	int	len;

	if (!name || !shell->env)
		return (NULL);
	len = ft_strlen(name);
	i = 0;
	while (shell->env[i])
	{
		if (ft_strncmp(shell->env[i], name, len) == 0
			&& shell->env[i][len] == '=')
			return (&shell->env[i][len + 1]);
		i++;
	}
	return (NULL);
}

static char	*update_the_value(char *temp, char *value, t_env_and_exit *shell,
		char *name)
{
	char	*new_var;
	int		i;

	new_var = ft_strjoin(temp, value);
	free(temp);
	if (!new_var)
	{
		shell->err = 1;
		return (NULL);
	}
	i = 0;
	while (shell->env[i])
	{
		if ((ft_strncmp(shell->env[i], name, ft_strlen(name)) == 0)
			&& (shell->env[i][ft_strlen(name)] == '='
			|| shell->env[i][ft_strlen(name)] == '\0'))
		{
			free(shell->env[i]);
			shell->env[i] = new_var;
			shell->err = 0;
			return (NULL);
		}
		i++;
	}
	return (new_var);
}

static char	**re_create_env(t_env_and_exit *shell, char *new_var)
{
	int		count;
	char	**new_env;
	int		i;

	count = 0;
	while (shell->env[count])
		count++;
	new_env = malloc(sizeof(char *) * (count + 2));
	if (!new_env)
	{
		free(new_var);
		return (NULL);
	}
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

int	set_env_value(char *name, char *value, t_env_and_exit *shell)
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
	{
		free(new_var);
		return (1);
	}
	free(shell->env);
	shell->env = new_env;
	return (0);
}
