/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtawil <mtawil@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 20:45:36 by mtawil            #+#    #+#             */
/*   Updated: 2025/12/02 18:29:38 by mtawil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*get_operator_token(const char *line, int *i)
{
	char	*token;

	if ((line[*i] == '>' && line[*i + 1] == '>') 
		|| (line[*i] == '<' && line[*i + 1] == '<'))
	{
		token = ft_strndup(line + *i, 2);
		*i += 2;
		return (token);
	}
	token = ft_strndup(line + *i, 1);
	(*i)++;
	return (token);
}

char	*get_quoted_token(const char *line, int *i, int *fla)
{
	char	quote;
	int		start;
	int		len;
	char	*token;

	quote = line[*i];
	if (quote == '"')
		*fla = 2;
	else if (quote == '\'')
		*fla = 1;
	else
		return (NULL);
	(*i)++;
	start = *i;
	while (line[*i] && line[*i] != quote)
		(*i)++;
	len = *i - start;
	token = ft_substr(line, start, len);
	if (line[*i] == quote)
		(*i)++;
	return (token);
}

char	*get_word_token(const char *line, int *i)
{
	int		start;
	char	c;

	start = *i;
	while (line[*i] && !ft_isspace(line[*i]) && !is_operator_start(line[*i]))
	{
		if (line[*i] == '"' || line[*i] == '\'')
		{
			c = line[*i];
			(*i)++;
			while (line[*i] && line[*i] != c)
				(*i)++;
			if (line[*i] == c)
				(*i)++;
		}
		else
			(*i)++;
	}
	return (ft_strndup(line + start, *i - start));
}

void	check_token(t_tokens *tokens)
{
	while (tokens)
	{
		if (ft_strcmp(tokens->value, "<") == 0)
			tokens->type = REDIR_IN;
		else if (ft_strcmp(tokens->value, ">") == 0)
			tokens->type = REDIR_OUT;
		else if (ft_strcmp(tokens->value, ">>") == 0)
			tokens->type = REDIR_APPEND;
		else if (ft_strcmp(tokens->value, "<<") == 0)
			tokens->type = REDIR_HEREDOC;
		else if (ft_strcmp(tokens->value, "|") == 0)
			tokens->type = TOKEN_PIPE;
		else
			tokens->type = TOKEN_WORD;
		tokens = tokens->next;
	}
}

t_tokens	*tokenize(const char *line, int *size)
{
	int			i;
	t_tokens	*tokens;
	char		*token;
	int			fla;
	t_tokens	*new_tok;

	i = 0;
	tokens = NULL;
	token = NULL;
	while (line[i])
	{
		while (ft_isspace(line[i]))
			i++;
		if (!line[i])
			break ;
		fla = 0;
		if (is_operator_start(line[i]))
			token = get_operator_token(line, &i);
		else
			token = get_word_token(line, &i);
		if (!token)
			break ;
		new_tok = create_token(token, fla);
		if (!new_tok)
			break ;
		add_token(&tokens, new_tok);
		(*size)++;
	}
	if (tokens)
		check_token(tokens);
	return (tokens);
}
