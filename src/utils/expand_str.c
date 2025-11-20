/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_str.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtawil <mtawil@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/20 02:38:22 by mtawil            #+#    #+#             */
/*   Updated: 2025/11/20 02:38:23 by mtawil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char *expand_string(char *str, t_shell *shell)
{
    char *result;
    char *temp;
    char *var_value;
    char *before_var;
    int i;
    int start;
    
    result = ft_strdup("");
    i = 0;
    start = 0;
    
    while (str[i])
    {
        if (str[i] == '$')
        {
            if (i > start)
            {
                before_var = ft_substr(str, start, i - start);
                temp = ft_strjoin(result, before_var);
                free(result);
                free(before_var);
                result = temp;
            }
            
            var_value = expand_variable(str, shell, &i);
            temp = ft_strjoin(result, var_value);
            free(result);
            free(var_value);
            result = temp;
            
            start = i;
        }
        else
            i++;
    }
    
    if (i > start)
    {
        before_var = ft_substr(str, start, i - start);
        temp = ft_strjoin(result, before_var);
        free(result);
        free(before_var);
        result = temp;
    }
    
    return (result);
}