/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtawil <mtawil@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 02:46:01 by mtawil            #+#    #+#             */
/*   Updated: 2026/02/14 13:34:52 by mtawil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	print_env_sorted(t_shell *shell)
{
	int		i;
	int		j;
	char	*temp;

	i = 0;
	while (shell->env[i])
	{
		j = i + 1;
		while (shell->env[j])
		{
			if (ft_strcmp(shell->env[i], shell->env[j]) > 0)
			{
				temp = shell->env[i];
				shell->env[i] = shell->env[j];
				shell->env[j] = temp;
			}
			j++;
		}
		i++;
	}
	i = 0;
	while (shell->env[i])
	{
		char **x = ft_split(shell->env[i++], '=');
		printf("declare -x %s", x[0]);
		if (x[1])
			printf("=\"%s\"", x[1]);
		printf("\n");
	}
	return (0);
}

int	builtin_export(char **args, t_shell *shell)
{
	char	*clean_value;
	int		ret;
	char	*name;
	int		i;

	i = 1;
	ret = 0;
	if (!args[1])
		return (print_env_sorted(shell));
	while (args[i])
	{
		name = get_name(args[i], shell, &ret, &i);
		if (!name)
			continue ;
		clean_value = get_value(args[i], name, &ret, &i);
		if (!clean_value)
			continue ;
		if (set_env(name, clean_value, shell) != 0)
			ret = 1;
		i++;
	}
	if (ret)
		ret = 1;
	return (ret);
}
