/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_helpers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtawil <mtawil@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/14 17:26:02 by mtawil            #+#    #+#             */
/*   Updated: 2026/02/14 17:56:19 by mtawil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	var_name_len(char *str)
{
	int	len;

	if (!str || !str[0])
		return (0);
	if (str[0] == '?')
		return (1);
	len = 0;
	while (str[len] && (ft_isalnum(str[len]) || str[len] == '_'))
		len++;
	return (len);
}

char	*var_value(char *after_dollar, int len, t_shell *shell)
{
	char	*key;
	char	*val;

	if (len == 1 && after_dollar[0] == '?')
		return (ft_itoa(shell->exit_code));
	key = ft_malloc(len + 1);
	if (!key)
		return (ft_strdup(""));
	ft_strlcpy(key, after_dollar, len + 1);
	val = get_env(key, shell->env);
	if (val)
		return (ft_strdup(val));
	return (ft_strdup(""));
}

int	count_expanded_len(char *str, t_shell *shell)
{
	int		i_str;
	int		res_len;
	int		name_len;
	char	quote;

	i_str = 0;
	res_len = 0;
	quote = 0;
	while (str[i_str])
	{
		if (!quote && (str[i_str] == '\'' || str[i_str] == '"'))
			quote = str[i_str];
		else if (quote && str[i_str] == quote)
			quote = 0;
		if (str[i_str] == '$' && quote != '\'' && var_name_len(str + i_str + 1))
		{
			name_len = var_name_len(str + i_str + 1);
			res_len += ft_strlen(var_value(str + i_str + 1, name_len, shell));
			i_str += 1 + name_len;
			continue ;
		}
		res_len++;
		i_str++;
	}
	return (res_len);
}
