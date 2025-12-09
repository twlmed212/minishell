/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtawil <mtawil@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/09 15:10:53 by mtawil            #+#    #+#             */
/*   Updated: 2025/12/09 15:13:28 by mtawil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_shell	*get_and_set_value(t_shell *original, int code)
{
	static t_shell	*current;

	if (original)
		current = original;
	if (code != -1)
		current->exit_code = code;
	return (current);
}

t_cmd	*get_pointer_cmds(t_cmd *original)
{
	static t_cmd	*current;

	if (original)
		current = original;
	return (current);
}

int	process_line(char *line, t_shell *shell)
{
	t_token	*tokens;
	t_cmd	*cmds;

	if (!line || !*line)
		return (0);
	if (check_unclosed_quotes(line) == -1)
		return (-1);
	add_history(line);
	tokens = lexer(line);
	if (!tokens)
		return (0);
	cmds = parser(tokens);
	get_pointer_cmds(cmds);
	free_tokens(tokens);
	if (!cmds)
		return (0);
	executor(cmds, shell);
	free_cmds(cmds);
	return (0);
}

char	*get_prompt(void)
{
	t_shell	*shell;
	char	*prompt;

	shell = get_and_set_value(NULL, -1);
	if (shell->exit_code != 0)
		prompt = "\001\033[31m\002minishell> \001\033[0m\002";
	else
		prompt = "\001\033[34m\002minishell> \001\033[0m\002";
	return (prompt);
}

int	check_signals(char *line, t_shell *shell)
{
	if (!line)
	{
		printf("exit\n");
		return (-1);
	}
	if (g_signal == 200 && line[0] != '\0')
	{
		ft_perror("Quit\n");
		exit(131);
	}
	if (g_signal == SIGINT)
	{
		shell->exit_code = 130;
	}
	return (0);
}
