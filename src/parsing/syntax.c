/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtawil <mtawil@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 02:46:25 by mtawil            #+#    #+#             */
/*   Updated: 2025/11/29 14:38:46 by mtawil           ###   ########.fr       */
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
	else if(ft_strcmp(tokens->value, ":") == 0 || ft_strcmp(tokens->value, "!") == 0)
		return (0);
	return (1);
}

static int	check_pipe_or_redir(t_tokens *current)
{
	if (current->type == TOKEN_PIPE)
	{
		if (!current->next)
		{
			ft_perror("minishell: syntax error near unexpected token `|'\n");
			return (0);
		}
	}
	if (current->type >= REDIR_IN && current->type <= REDIR_HEREDOC)
	{
		if (!current->next || current->next->type != TOKEN_WORD)
		{
			// CRITICAL FIX: The issue is HERE!
			// When current->value is ">", ft_strlen is 1, not 2
			// So ft_substr(">", 0, 2) tries to read beyond the string!
			
			if (current->type != REDIR_HEREDOC && current->next && current->next->type != TOKEN_WORD)
			{
				// This is for cases like "> >" (redirect to another redirect)
				ft_perror("minishell: syntax error near unexpected token `");
				ft_perror(current->next->value);
				ft_perror("'\n");
			}
			else if (!current->next)
			{
				// This is for cases like "> " at end of line
				ft_perror("minishell: syntax error near unexpected token `newline'\n");
			}
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