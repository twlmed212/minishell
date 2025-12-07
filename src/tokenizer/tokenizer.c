/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtawil <mtawil@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 20:45:36 by mtawil            #+#    #+#             */
/*   Updated: 2025/12/07 16:47:48 by mtawil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_token	*new_token(char *value, t_token_type type)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->value = value;
	token->type = type;
	token->next = NULL;
	return (token);
}

void	add_token(t_token **head, t_token *new)
{
	t_token	*tmp;

	if (!*head)
	{
		*head = new;
		return ;
	}
	tmp = *head;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
}

static int	is_space(char c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}

static int	is_special(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

char	*extract_word(char *line, int *i)
{
	int		start;
	int		len;
	char	*word;
	char	quote;

	start = *i;
	quote = 0;
	while (line[*i] && (quote || (!is_space(line[*i]) 
		&& !is_special(line[*i]))))
	{
		if (!quote && (line[*i] == '\'' || line[*i] == '"'))
			quote = line[*i];
		else if (quote && line[*i] == quote)
			quote = 0;
		(*i)++;
	}
	len = *i - start;
	word = malloc(len + 1);
	if (!word)
		return (NULL);
	strncpy(word, line + start, len);
	word[len] = '\0';
	return (word);
}