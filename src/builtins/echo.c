/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtawil <mtawil@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 02:45:54 by mtawil            #+#    #+#             */
/*   Updated: 2025/12/07 22:05:40 by mtawil           ###   ########.fr       */
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
int	skip_and_apply_n_flags(char **args, int *newline)
{
	int	i;
	int	k;

	i = 1;
	while (args[i] && (ft_strncmp(args[i], "-n", 2) == 0))
	{
		k = 2;
		while (args[i][++k] == 'n')
			k++;
		if (args[i][k] == '\0')
			*newline = 0;
		i++;
	}
	return (i);
}
int	builtin_echo(char **args, t_shell *shell)
{
	int	i;
	int	newline;

	newline = 1;
	i = skip_and_apply_n_flags(args, &newline);
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
