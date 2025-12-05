/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtawil <mtawil@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/14 02:45:27 by mtawil            #+#    #+#             */
/*   Updated: 2025/12/05 12:17:08 by mtawil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void		rl_clear_history(void);

t_env_and_exit *get_and_set_value(t_env_and_exit *original, int code)
{
	static t_env_and_exit *current;
	if (original)
		current = original;
	if (code)
		current->last_exit = code;
	return (current);
}
static void	init_env_and_signals(t_env_and_exit *shell, char **env)
{
	shell->env = copy_env(env);
	if (!shell->env)
	{
		printf("Error: Failed to copy environment\n");
		exit(1);
	}
	get_and_set_value(shell, 0);
}

static int	read_input(char **input, t_env_and_exit *shell)
{
	*input = readline("minishell> ");
	g_signal = 0;
	if (!*input)
	{
		ft_perror("exit\n");
		return (-1);
	}
	if (g_signal == SIGINT)
	{
		shell->last_exit = 130;
		g_signal = 0;
		free(*input);
		return (1);
	}
	if (!(*input)[0])
	{
		free(*input);
		return (1);
	}
	if (check_unclosed_quotes(*input) == -1)
		return (1);
	return (0);
}

int	main(int ac, char **av, char **env)
{
	char			*input;
	int				status;
	t_env_and_exit	shell;

	(void)ac;
	(void)av;
	init_env_and_signals(&shell, env);
	while (1)
	{
		init_signals();
		status = read_input(&input, &shell);
		if (status == -1)
			break ;
		else if (status == 1)
			continue ;
		execute_command(input, &shell);
		add_history(input);
		free(input);
	}
	free_array(shell.env);
	rl_clear_history();
	return (0);
}
