/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper_functinos.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtawil <mtawil@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/09 13:01:13 by mtawil            #+#    #+#             */
/*   Updated: 2026/02/12 20:42:35 by mtawil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int ft_isspace(char ch)
{
    return (ch == ' ' || ch == '\t' || ch == '\n');
}

void	skip_spaces(char *str, int *i)
{
	while (str[*i] && ft_isspace(str[*i]))
		(*i)++;
}


static int	is_special_char(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

static int	word_size(char *str)
{
	int		len;
	char	quote_state;

	len = 0;
	quote_state = 0;
	while (str[len])
	{
		if (quote_state == 0)
		{
			if (str[len] == '\'' || str[len] == '"')
				quote_state = str[len];
			else if (ft_isspace(str[len]) || is_special_char(str[len]))
				break ;
		}
		else
		{
			if (str[len] == quote_state)
				quote_state = 0;
		}
		len++;
	}
	return (len);
}

char	*extract_word(char *str, int *i)
{
	char	*word;
	char	*expanded_word;
	char	*without_qoutes;
	int		len;

	len = word_size(str + *i);
	if (len == 0)
		return (NULL);

	word = ft_malloc(sizeof(char) * (len + 1));
	if (!word)
		return (NULL);
	ft_strlcpy(word, str + *i, len + 1);

	*i += len;
	expanded_word = expand(word);

	without_qoutes = remove_qoutes(expanded_word);
	return (without_qoutes);
}

t_redir_type	get_redir_type(char *str, int *i)
{
	if (str[*i] == '<')
	{
		if (str[*i + 1] == '<')
		{
			*i += 2;
			return (T_HEREDOC);
		}
		(*i)++;
		return (T_REDIR_IN);
	}
	if (str[*i + 1] == '>')
	{
		*i += 2;
		return (T_APPEND);
	}
	(*i)++;
	return (T_REDIR_OUT);
}
