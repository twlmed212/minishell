/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtawil <mtawil@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 16:45:35 by mtawil            #+#    #+#             */
/*   Updated: 2025/12/08 12:19:35 by mtawil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static char	*try_path(char *dir, char *cmd)
{
	char	*path;
	int		len;

	len = ft_strlen(dir) + ft_strlen(cmd) + 2;
	path = malloc(len);
	if (!path)
		return (NULL);
	strcpy(path, dir);
	strcat(path, "/");
	strcat(path, cmd);
	if (access(path, X_OK) == 0)
		return (path);
	free(path);
	return (NULL);
}

static char	*look_for_path(char *cmd, char **env)
{
	char	**paths;
	char	*path;
	int		i;

	path = get_env("PATH", env);
	if (!path)
		return (NULL);
	paths = ft_split(path, ':');
	if (!paths)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		path = try_path(paths[i], cmd);
		if (path)
		{
			free_array(paths);
			return (path);
		}
		i++;
	}
	free_array(paths);
	return (NULL);
}

char	*find_path(char *cmd, char **env)
{
	if (!cmd)
		return (NULL);
	if (strchr(cmd, '/'))
	{
		if (access(cmd, X_OK) == 0)
			return (ft_strdup(cmd));
		else
			return (NULL);
	}
	return (look_for_path(cmd, env));
}
