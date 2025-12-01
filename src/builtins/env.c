/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtawil <mtawil@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 02:45:56 by mtawil            #+#    #+#             */
/*   Updated: 2025/11/30 15:02:01 by mtawil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	builtin_env(char **args, t_env_and_exit *shell)
{
	int	i;

	(void)args;
	i = 0;
	while (shell->env[i])
	{
		if (ft_strchr(shell->env[i], '=') != NULL)
			printf("%s\n", shell->env[i]);
		i++;
	}
	return (0);
}
