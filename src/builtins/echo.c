/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtawil <mtawil@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 02:45:54 by mtawil            #+#    #+#             */
/*   Updated: 2025/12/07 16:42:00 by mtawil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static void	print_arg(char *str, t_shell *shell)
{
	if (ft_strcmp(str, "$?") == 0)
		printf("%d", shell->exit_code);
	else
		printf("%s", str);
}

int	builtin_echo(char **args, t_shell *shell)
{
	int	i;
	int	newline;

	newline = 1;
	i = 1;
	if (args[1] && ft_strcmp(args[1], "-n") == 0)
	{
		newline = 0;
		i = 2;
	}
	while (args[i])
	{
		print_arg(args[i], shell);
		if (args[i + 1])
			printf(" ");
		i++;
	}
	if (newline)
		printf("\n");
	return (0);
}
