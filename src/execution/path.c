/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtawil <mtawil@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/07 16:45:35 by mtawil            #+#    #+#             */
/*   Updated: 2026/02/12 15:53:44 by mtawil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*try_path(char *dir, char *cmd, char **first_file)
{
	char	*path;
	int		len;

	len = ft_strlen(dir) + ft_strlen(cmd) + 2;
	path = ft_malloc(len);
	if (!path)
		return (NULL);
	ft_strlcpy(path, dir, len);
	ft_strlcat(path, "/", len);
	ft_strlcat(path, cmd, len);
	if (access(path, F_OK) == 0 && !*first_file)
		*first_file = path;
	if (access(path, X_OK) == 0)
		return (path);
	return (NULL);
}

static char	*look_for_path(char *cmd, char **env)
{
	char	**paths;
	char	*path;
	char	*first_file;
	int		i;

	path = get_env("PATH", env);
	if (!path)
		return (NULL);
	paths = ft_split(path, ':');
	if (!paths)
		return (NULL);
	i = 0;
	first_file = NULL;
	while (paths[i])
	{
		path = try_path(paths[i], cmd, &first_file);
		if (path)
			return (path);
		i++;
	}
	return (first_file);
}

char	*find_path(char *cmd, char **env)
{
	if (!cmd)
		return (NULL);
	if (ft_strchr(cmd, '/'))
	{
		return (ft_strdup(cmd));
	}
	return (look_for_path(cmd, env));
}
