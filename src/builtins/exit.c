/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtawil <mtawil@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 02:45:59 by mtawil            #+#    #+#             */
/*   Updated: 2025/12/08 22:13:11 by mtawil           ###   ########.fr       */
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
	unsigned char	exit_code;

	printf("exit\n");
	exit_code = shell->exit_code;
	if (cmd->args[1])
	{
		if (!is_numeric(cmd->args[1]))
		{
			ft_perror("exit: ");
			ft_perror(cmd->args[1]);
			ft_perror(": numeric argument required\n");
			exit_code = 2;
		}
		else if (cmd->args[2])
		{
			ft_perror("exit: too many arguments\n");
			shell->exit_code = 1;
			return ;
		}
		else
			exit_code = ft_atoi(cmd->args[1]);
	}
	if (cmd)
		free_cmds(cmd);
	free_array(shell->env);
	exit(exit_code);
}
