/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtawil <mtawil@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/30 14:15:00 by mtawil            #+#    #+#             */
/*   Updated: 2025/12/07 19:34:08 by mtawil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	expand_exit_code(char **args, int exit_code)
{
	int		i;
	char	*tmp;
	char	*res;

	i = 0;
	while (args[i])
	{
		if (ft_strncmp(args[i], "$?", 2) == 0)
		{
			tmp = ft_itoa(exit_code);
			res = ft_strjoin(tmp, args[i] + 2);
			free(args[i]);
			args[i] = res;
			free(tmp);
		}
		i++;
	}
}

int	handle_dir_error(int stat, char **args, t_env_and_exit *shell)
{
	ft_perror("minishell: ");
	ft_perror(args[0]);
	if (stat == -1)
	{
		shell->last_exit = 127;
		return (ft_perror(": No such file or directory\n"), -1);
	}
	else if (stat == 1)
		return (ft_perror(": Is a directory\n"), -1);
	else if (stat == 2)
		return (ft_perror(": Not a directory\n"), -1);
	return (0);
}

int	look_for_directories(char *args)
{
	struct stat	metadata;

	if (args[0] == '/')
		return (-1);
	if (stat(args, &metadata) == 0)
	{
		if (S_ISDIR(metadata.st_mode))
			return (1);
		else if (S_ISREG(metadata.st_mode))
			return (2);
	}
	if (args[ft_strlen(args) - 1] == '/')
		return (-1);
	return (0);
}

int	handle_heredoc_failure(char **args)
{
	free_array(args);
	return (-1);
}

int	preprocess_heredocs(char **args)
{
	int				i;
	char			*temp_file;
	char			*old_delimiter;
	t_env_and_exit	*shell;

	shell = NULL;
	i = 0;
	shell = get_and_set_value(NULL, -1);
	while (args[i])
	{
		if (ft_strcmp(args[i], "<<") == 0 && args[i + 1])
		{
			old_delimiter = args[i + 1];
			temp_file = read_heredoc(old_delimiter);
			init_signals();
			free(old_delimiter);
			if (!temp_file)
				return (-1);
			args[i + 1] = temp_file;
			if (shell->last_exit == 130)
				return (-1);
		}
		i++;
	}
	return (0);
}