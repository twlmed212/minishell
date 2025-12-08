/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtawil <mtawil@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 20:45:36 by mtawil            #+#    #+#             */
/*   Updated: 2025/12/08 12:05:48 by mtawil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_token				*new_token(char *value, t_token_type type);
void				add_token(t_token **head, t_token *new);
char				*extract_word(char *line, int *i);

static t_token_type	get_operator_type(char *line, int *i)
{
	if (line[*i] == '|')
	{
		(*i)++;
		return (T_PIPE);
	}
	if (line[*i] == '<')
	{
		if (line[*i + 1] == '<')
		{
			(*i) += 2;
			return (T_HEREDOC);
		}
		(*i)++;
		return (T_REDIR_IN);
	}
	if (line[*i] == '>')
	{
		if (line[*i + 1] == '>')
		{
			(*i) += 2;
			return (T_APPEND);
		}
		return ((*i)++, T_REDIR_OUT);
	}
	return (T_WORD);
}

static t_token	*create_operator_token(char *line, int *i)
{
	t_token_type	type;
	char			*value;

	type = get_operator_type(line, i);
	if (type == T_PIPE)
		value = ft_strdup("|");
	else if (type == T_REDIR_IN)
		value = ft_strdup("<");
	else if (type == T_REDIR_OUT)
		value = ft_strdup(">");
	else if (type == T_APPEND)
		value = ft_strdup(">>");
	else
		value = ft_strdup("<<");
	return (new_token(value, type));
}

static int	is_space(char c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}

static int	is_special(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

t_token	*lexer(char *line)
{
	t_token	*tokens;
	t_token	*new;
	int		i;

	tokens = NULL;
	i = 0;
	while (line[i])
	{
		while (is_space(line[i]))
			i++;
		if (!line[i])
			break ;
		if (is_special(line[i]))
			new = create_operator_token(line, &i);
		else
			new = new_token(extract_word(line, &i), T_WORD);
		if (!new)
		{
			free_tokens(tokens);
			return (NULL);
		}
		add_token(&tokens, new);
	}
	return (tokens);
}
