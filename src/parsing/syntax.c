/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtawil <mtawil@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/12 13:48:32 by mtawil            #+#    #+#             */
/*   Updated: 2026/02/12 20:50:51 by mtawil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int check_pipe(char *str, int *i){
	if (str[*i] == '|')
	{
		(*i)++;
		skip_spaces(str, i);
		if (!str[*i] || str[*i] == '|')
		{
			ft_perror("minishell: syntax error near unexpected token `|'\n");
			return (1);
		}
	}
	return (0);
}

static int check_redir(char *str, int *i){
	if (str[*i] == '<' || str[*i] == '>')
	{
		(*i)++;
		if (str[*i] == '<' || str[*i] == '>')
			(*i)++;
		skip_spaces(str, i);
		if (!str[*i] || str[*i] == '|' || str[*i] == '<' || str[*i] == '>')
		{
			ft_perror("minishell: syntax error near unexpected token `newline'\n");
			return (1);
		}
	}
	return (0);
}
int	check_syntax_error(char *str)
{
	int	i;

	i = 0;
	skip_spaces(str, &i);
	if (str[i] == '|')
	{
		ft_perror("minishell: syntax error near unexpected token `|'\n");
		return (1);
	}
	while (str[i])
	{
		if (check_pipe(str, &i))
			return (1);
		else if (check_redir(str, &i))
			return (1);
		else 
			i++;
	}
	return (0);
}
