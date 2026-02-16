/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper_functinos_two.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtawil <mtawil@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/14 17:58:13 by mtawil            #+#    #+#             */
/*   Updated: 2026/02/16 09:57:21 by mtawil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_isspace(char ch)
{
	return (ch == ' ' || ch == '\t' || ch == '\n');
}

int	skip_spaces(char *str, int *i)
{
	while (str[*i] && ft_isspace(str[*i]))
		(*i)++;
	return (str[*i]);
}

int	is_special_char(char c)
{
	return (c == '|' || c == '<' || c == '>');
}
