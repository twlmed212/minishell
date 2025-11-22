/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtawil <mtawil@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 02:45:54 by mtawil            #+#    #+#             */
/*   Updated: 2025/11/22 06:58:27 by mtawil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	builtin_echo(char **args)
{
	int i;
	int newline;

	newline = 1;
	i = 1;

	if (args[1] && (ft_strcmp(args[1], "-n") == 0 && ft_strlen(args[1]) == 2))
	{
		newline = 0;
		i = 2;
	}

	while (args[i])
	{
		printf("%s", args[i]);
		if (args[i + 1])
			printf(" ");
		i++;
	}

	if (newline)
		printf("\n");

	return (0);
}
