/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtawil <mtawil@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/09 15:10:53 by mtawil            #+#    #+#             */
/*   Updated: 2026/02/14 16:30:02 by mtawil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_shell	*get_and_set_value(t_shell *original, int code)
{
	static t_shell	*current;

	if (original)
		current = original;
	if (code != -1)
		current->exit_code = code;
	return (current);
}

int	process_line(char *line, t_shell *shell)
{
	t_cmd	*cmds;
	t_cmd	*cmd_list;
	t_cmd	*current;

	cmd_list = NULL;

	if (!line || !*line)
		return (1);
	if (check_unclosed_qoutes(line) == 1)
		return (0);
	add_history(line);
	if (check_syntax_error(line))
		return (0);
	current = alloc_cmd();
	if (!current)
		return (0);
	cmds = parser(line, &cmd_list, &current);
	if (!cmds)
		return (0);
	
	executor(cmds, shell);
	return (0);
}

int	check_signals(char *line, t_shell *shell)
{
	if (!line)
	{
		ft_perror("exit\n");
		return (-1);
	}
	if (g_signal == SIGQUIT)
	{
		shell->exit_code = 131;
	}else if (g_signal == SIGINT)
	{
		shell->exit_code = 130;
	}
	return (0);
}
