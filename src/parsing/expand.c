/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtawil <mtawil@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/09 15:10:53 by mtawil            #+#    #+#             */
/*   Updated: 2026/02/14 17:55:50 by mtawil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	write_var(char *str, char *res, t_shell *shell)
{
	int		name_len;
	char	*val;

	name_len = var_name_len(str + 1);
	val = var_value(str + 1, name_len, shell);
	ft_strlcpy(res, val, ft_strlen(val) + 1);
	return (ft_strlen(val));
}

static void	fill_expanded(char *str, char *res, t_shell *shell)
{
	int		i_str;
	int		i_res;
	char	qoute;

	i_str = 0;
	i_res = 0;
	qoute = 0;
	while (str[i_str])
	{
		if (!qoute && (str[i_str] == '\'' || str[i_str] == '"'))
			qoute = str[i_str];
		else if (qoute && str[i_str] == qoute)
			qoute = 0;
		if (str[i_str] == '$' && qoute != '\'' && var_name_len(str + i_str + 1))
		{
			i_res += write_var(str + i_str, res + i_res, shell);
			i_str += 1 + var_name_len(str + i_str + 1);
			continue ;
		}
		res[i_res++] = str[i_str++];
	}
	res[i_res] = '\0';
}

char	*expand(char *str)
{
	char	*result;
	t_shell	*shell;

	if (!ft_strchr(str, '$'))
		return (str);
	shell = get_and_set_value(NULL, -1);
	result = ft_malloc(count_expanded_len(str, shell) + 1);
	if (!result)
		return (str);
	fill_expanded(str, result, shell);
	return (result);
}
