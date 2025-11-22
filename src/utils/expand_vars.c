/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_vars.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtawil <mtawil@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/20 02:38:30 by mtawil            #+#    #+#             */
/*   Updated: 2025/11/22 05:01:29 by mtawil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*expand_variable(char *str, t_env_and_exit *shell, int *pos)
{
	char	*var_name;
	char	*value;
	int		var_len;

	var_name = extract_var_name(str + *pos);
	if (!var_name)
	{
		(*pos)++;
		return (ft_strdup("$"));
	}
	if (ft_strcmp(var_name, "?") == 0)
	{
		value = ft_itoa(shell->last_exit);
		free(var_name);
		*pos += 2;
		return (value);
	}
	value = get_env_value(var_name, shell);
	var_len = ft_strlen(var_name);
	*pos += var_len + 1;
	free(var_name);
	if (!value)
		return (ft_strdup(""));
	return (ft_strdup(value));
}
