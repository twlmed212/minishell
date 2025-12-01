/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtawil <mtawil@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 02:46:19 by mtawil            #+#    #+#             */
/*   Updated: 2025/12/01 23:04:35 by mtawil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	is_executable(char *path)
{
	return (access(path, X_OK) == 0);
}

static char	*get_full_path(char **dirs, char *cmd, int index)
{
	char	*temp;
	char	*full_path;

	temp = ft_strjoin(dirs[index], "/");
	if (!temp)
		return (NULL);
	full_path = ft_strjoin(temp, cmd);
	free(temp);
	if (!full_path)
		return (free_array(dirs), NULL);
	if (is_executable(full_path))
	{
		free_array(dirs);
		return (full_path);
	}
	free(full_path);
	return (NULL);
}

char	*search_for_path(char *path_env, char *cmd, char **full_path)
{
	char	**dirs;
	int		i;

	dirs = ft_split(path_env, ':');
	if (!dirs)
		return (NULL);
	i = 0;
	while (dirs[i])
	{
		*full_path = get_full_path(dirs, cmd, i);
		if (*full_path)
			return (*full_path);
		i++;
	}
	free_array(dirs);
	return (NULL);
}

char	*find_command_path(char *cmd, t_env_and_exit *shell)
{
	char	*path_env;
	char	*full_path;

	full_path = NULL;
	if (ft_strchr(cmd, '/'))
	{
		if (is_executable(cmd))
			return (ft_strdup(cmd));
		return (NULL);
	}
	path_env = get_env_value("PATH", shell);
	if (!path_env)
		return (NULL);
	full_path = search_for_path(path_env, cmd, &full_path);
	return (full_path);
}
