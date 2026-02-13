/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtawil <mtawil@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/09 15:10:53 by mtawil            #+#    #+#             */
/*   Updated: 2026/02/13 00:14:01 by mtawil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int skp_untl_spc_dllr(char *str, int i){
	while(str[i] && (!ft_isspace(str[i]) && str[i] != '$'))
		i++;
	return (i);
}

static int	calc_new_length(char *str, int expand_len)
{
	int	len;
	int	i;

	len = 0;
	i = 0;
	while (str[i])
	{
		if (str[i] == '$')
		{
			len += expand_len;
			i += skp_untl_spc_dllr(str+i, i);
		}
		else
		{
			len++;
			i++;
		}
	}
	return (len);
}

static void	fill_expanded(char *str, char *result, char *expanded)
{
	int	i_str;
	int	i_result;
	int	i_expanded;

	i_str = 0;
	i_result = 0;
	int found = 1;
	while (str[i_str])
	{
		if (str[i_str] == '$' && found)
		{
			i_expanded = 0;
			while (expanded[i_expanded])
				result[i_result++] = expanded[i_expanded++];
			if (str[i_str + 1] == '?')
				i_str += 2;
			else
				i_str += skp_untl_spc_dllr(str+i_str, 0);
			found = 0;
		}else
			result[i_result++] = str[i_str++];
	}
	result[i_result] = '\0';
}

char	*expand(char *str)
{
	char	*expanded;
	char	*result;
	int		result_len;
	t_shell	*shell;
    char *expand_start;
	
    expand_start = ft_strchr(str, '$');
	if (!expand_start)
		return (str);
	shell = get_and_set_value(NULL, -1);

	if (*(expand_start + 1) == '?'){
		expanded = ft_itoa(shell->exit_code);
	}else{
		expanded = get_env(expand_start + 1, shell->env);
	}
	if (!expanded)
		return (str);
	result_len = calc_new_length(str, ft_strlen(expanded));
	result = ft_malloc(sizeof(char) * (result_len + 1));
	if (!result)
		return (str);
    fill_expanded(str, result, expanded);
	return (expand(result));
}
