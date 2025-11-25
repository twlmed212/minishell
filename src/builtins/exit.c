/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtawil <mtawil@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 02:45:59 by mtawil            #+#    #+#             */
/*   Updated: 2025/11/25 15:17:19 by mtawil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"


static int	ft_help(char **args, int i, int sign)
{
	int	j;

	j = i;
	while (args[1][i])
	{
		if (!isdigit(args[1][i]))
			return (-1);
		i++;
	}
	if (args[2])
		return (-2);
	i = atoi(args[1] + j) * sign;
	return ((unsigned char)(i));
}

static int	ft_exit_check(char **args)
{
	int	i;
	int	sign;
	int	res;

	i = 0;
	sign = 1;
	while (args[1][i] == ' ' || (args[1][i] >= 9 && args[1][i] <= 13))
		i++;
	if (args[1][i] == '-' || args[1][i] == '+')
		if (args[1][i++] == '-')
			sign = -1;
	while (args[1][i] == '0')
		i++;
	res = strlen(args[1] + i);
	if (res > 19)
		return (-1);
	if (res == 19)
	{
		if (sign != -1 && ft_strcmp(args[1] + i, MAX_EXIT) > 0)
			return (-1);
		if (sign == -1 && ft_strcmp(args[1] + i, MIN_EXIT) > 0)
			return (-1);
	}
	return (ft_help(args, i, sign));
}

void builtin_exit(char **args, t_env_and_exit *shell)
{
    int flag;
    rl_clear_history();

    printf("exit\n");
    
	if (!args[1])
	{
        // Free environment before exit
        if (shell && shell->env)
            free_array(shell->env);
        // Note: args will be freed by caller
        exit(0);
    }

    flag = ft_exit_check(args);

    if (flag == -1)
    {
        printf("minishell: %s: numeric argument required\n", args[1]);
        flag = 2;
    }
    else if (flag == -2)
    {
        printf("exit: too many arguments\n");
        return;
    }

    // Free before exit!
    free_array(args);
    if (shell && shell->env)
        free_array(shell->env);
    
    exit(flag);
}
