/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtawil <mtawil@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 02:46:25 by mtawil            #+#    #+#             */
/*   Updated: 2025/11/28 21:09:35 by mtawil           ###   ########.fr       */
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
	}else if(ft_strcmp(tokens->value, ":") == 0 || ft_strcmp(tokens->value, "!") == 0)
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
            if (ft_strlen(current->value) == 2 && current->type != REDIR_HEREDOC)
            {
                char *tmp = ft_substr(current->value, 0, 2);
                ft_perror("minishell: syntax error near unexpected token '");
                ft_perror(tmp);
                ft_perror("'\n");
                free(tmp);
            }else if (current->type != REDIR_HEREDOC && (current->next)->type != TOKEN_WORD)
                ft_perror("minishell: syntax error near unexpected token `newline'\n");
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