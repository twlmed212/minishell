/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtawil <mtawil@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/14 02:45:27 by mtawil            #+#    #+#             */
/*   Updated: 2025/12/11 18:25:12 by mtawil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	main(int ac, char **av, char **env)
{
	t_shell	shell;
	char	*line;

	(void)ac;
	(void)av;
	shell.env = copy_env(env);
	get_and_set_value(&shell, -1);
	shell.exit_code = 0;
	while (1)
	{
		setup_signals();
		line = readline("minishell> ");
		if (check_signals(line, &shell) == -1)
			break ;
		if (process_line(line, &shell) == -1)
			continue ;
		free(line);
		g_signal = 0;
	}
	free_array(shell.env);
	return (shell.exit_code);
}
