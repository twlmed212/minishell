/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtawil <mtawil@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 02:46:25 by mtawil            #+#    #+#             */
/*   Updated: 2025/12/02 18:29:29 by mtawil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	check_first_token(t_tokens *tokens)
{
	if (!tokens)
	{
		printf("Empty command.\n");
		return (0);
	}
	if (tokens->type == TOKEN_PIPE)
	{
		ft_perror("minishell: syntax error near unexpected token `|'\n");
		return (0);
	}
	else if (ft_strcmp(tokens->value, ":") == 0 || ft_strcmp(tokens->value,
			"!") == 0)
		return (0);
	return (1);
}

static int	check_pipe_or_redir(t_tokens *current)
{
	if (current->type == TOKEN_PIPE)
	{
		if (!current->next)
			return (ft_perror("minishell: syntax error near\
					unexpected token `|'\n"), 0);
	}
	if (current->type >= REDIR_IN && current->type <= REDIR_HEREDOC)
	{
		if (!current->next || current->next->type != TOKEN_WORD)
		{
			if (current->type != REDIR_HEREDOC && current->next
				&& current->next->type != TOKEN_WORD)
			{
				ft_perror("minishell: syntax error near unexpected token `");
				ft_perror(current->next->value);
				ft_perror("'\n");
			}
			else if (!current->next)
				ft_perror("minishell: syntax error near\
						unexpected token `newline'\n");
			return (0);
		}
	}
	return (1);
}

int	check_simple_command(t_tokens *tokens)
{
	t_tokens	*current;

	if (!check_first_token(tokens))
		return (0);
	current = tokens;
	while (current)
	{
		if (!check_pipe_or_redir(current))
			return (0);
		current = current->next;
	}
	return (1);
}
