/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtawil <mtawil@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 02:46:01 by mtawil            #+#    #+#             */
/*   Updated: 2025/11/17 00:11:58 by mtawil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int builtin_export(char **args, t_shell *shell)
{
    char    *equal_sign;
    char    *name;
    char    *value;
    int     i;
    
    if (!args[1])
    {
        i = 0;
        while (shell->env[i])
        {
            printf("declare -x %s\n", shell->env[i]);
            i++;
        }
        return (0);
    }
    
    i = 1;
    while (args[i])
    {
        equal_sign = ft_strchr(args[i], '=');
        
        if (!equal_sign)
        {
            i++;
            continue;
        }
        
        name = ft_substr(args[i], 0, equal_sign - args[i]);
        value = ft_strdup(equal_sign + 1);
        
        if (!name || !value)
        {
            free(name);
            free(value);
            return (1);
        }
        
        set_env_value(name, value, shell);
        free(name);
        free(value);
        i++;
    }
    
    return (0);
}