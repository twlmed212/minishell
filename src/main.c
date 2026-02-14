/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtawil <mtawil@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/14 02:45:27 by mtawil            #+#    #+#             */
/*   Updated: 2026/02/14 13:39:42 by mtawil           ###   ########.fr       */
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
	setup_signals();
	while (1)
	{
		signal(SIGQUIT, SIG_IGN );
		line = readline("minishell> ");
		if (check_signals(line, &shell) == -1)
			break ;
		if (process_line(line, &shell))
			continue ;
		free(line);
		g_signal = 0;
	}
	free_grabage();
	return (shell.exit_code);
}
