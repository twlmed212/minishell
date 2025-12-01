/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_utils_pars.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtawil <mtawil@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/30 14:18:11 by mtawil            #+#    #+#             */
/*   Updated: 2025/12/01 20:39:40 by mtawil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static char	**tokens_to_array(t_tokens *tokens, int size, int exit_code)
{
	char		**args;
	int			i;
	t_tokens	*current;

	args = malloc(sizeof(char *) * (size + 1));
	if (!args)
		return (NULL);
	i = 0;
	current = tokens;
	while (current)
	{
		args[i] = ft_strdup(current->value);
		if (!args[i])
			return (cleanup_on_error(args, i), NULL);
		i++;
		current = current->next;
	}
	args[i] = NULL;
	expand_exit_code(args, exit_code);
	return (args);
}

static int	parse_cmd_dirs(char **args, t_env_and_exit *shell)
{
	int	stat;

	if (access(args[0], X_OK) == 0)
		return (0);
	if (args[0][0] != '/' && args[0][ft_strlen(args[0]) - 1] != '/')
		return (0);
	stat = look_for_directories(args[0]);
	if (stat == 0)
		return (0);
	if ((ft_strcmp(args[0], "/") == 0) || (ft_strcmp(args[0], "//") == 0)
		|| (ft_strcmp(args[0], "/.") == 0))
		stat = 1;
	return (handle_dir_error(stat, args, shell));
}

char	**parse_and_validate(char *command, t_env_and_exit *shell, int *size)
{
	t_tokens	*tokens;
	char		**args;

	tokens = tokenize(command, size);
	if (!tokens)
		return (NULL);
	if (check_simple_command(tokens) == 0)
		return (free_token_structs(tokens), NULL);
	args = tokens_to_array(tokens, *size, shell->last_exit);
	free_token_structs(tokens);
	if (!args || !args[0])
		return (free_array(args), NULL);
	if (parse_cmd_dirs(args, shell) == -1)
		return (free_array(args), NULL);
	if (has_pipe(args, shell))
		return (free_array(args), NULL);
	return (args);
}

void	print_cmd_error(char *cmd, t_env_and_exit *shell)
{
	ft_perror("minishell: ");
	if (!get_env_value("PATH", shell))
	{
		ft_perror(cmd);
		ft_perror(": No such file or directory\n");
	}
	else if (ft_strcmp(cmd, "sudo") == 0)
		ft_perror("/usr/bin/sudo: Permission denied\n");
	else
	{
		ft_perror(cmd);
		ft_perror(": command not found\n");
	}
}
