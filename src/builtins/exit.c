/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtawil <mtawil@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 02:45:59 by mtawil            #+#    #+#             */
/*   Updated: 2025/12/09 15:00:24 by mtawil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	is_numeric(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '+' || str[i] == '-')
		i++;
	if (!str[i])
		return (0);
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

void	builtin_exit(t_cmd *cmd, t_shell *shell)
{
	printf("exit\n");
	if (cmd->args[1])
	{
		if (!is_numeric(cmd->args[1]))
		{
			ft_perror("exit: ");
			ft_perror(cmd->args[1]);
			ft_perror(": numeric argument required\n");
			shell->exit_code = 2;
		}
		else if (cmd->args[2])
		{
			ft_perror("exit: too many arguments\n");
			shell->exit_code = 1;
			return ;
		}
		else
			shell->exit_code = ft_atoi(cmd->args[1]);
	}
	if (cmd)
		free_cmds(cmd);
	free_array(shell->env);
	exit(shell->exit_code);
}
