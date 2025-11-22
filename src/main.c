/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtawil <mtawil@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/14 02:45:27 by mtawil            #+#    #+#             */
/*   Updated: 2025/11/22 06:51:27 by mtawil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	main(int ac, char **av, char **env)
{
	char			*input;
	t_env_and_exit	shell;

	(void)ac;
	(void)av;
	shell.env = copy_env(env);
	if (!shell.env)
	{
		printf("Error: Failed to copy environment\n");
		return (1);
	}
	shell.last_exit = 0;
	while (1)
	{
		input = readline("minishell> ");
		if (!input)
		{
			printf("exit\n");
			break ;
		}
		if (input[0] == '\0')
		{
			free(input);
			continue ;
		}
		execute_command(input, &shell);
		add_history(input);
		free(input);
	}
	free_array(shell.env);
	return (0);
}
