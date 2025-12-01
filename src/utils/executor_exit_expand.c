/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor_exit_expand.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtawil <mtawil@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/30 14:19:49 by mtawil            #+#    #+#             */
/*   Updated: 2025/12/01 20:38:24 by mtawil           ###   ########.fr       */
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
