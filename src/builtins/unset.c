/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtawil <mtawil@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 02:46:05 by mtawil            #+#    #+#             */
/*   Updated: 2025/11/22 06:15:26 by mtawil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int builtin_unset(char **args, t_env_and_exit *shell)
{
    int i;
    
    if (!args[1])
        return (0);
    
    i = 1;
    while (args[i])
    {
        unset_env_value(args[i], shell);
        i++;
    }
    
    return (0);
}

int unset_env_value(char *name, t_env_and_exit *shell)
{
    int     i;
    int     j;
    int     len;
    char    **new_env;
    int     count;
    
    len = ft_strlen(name);
    
    i = 0;
    while (shell->env[i])
    {
        if (ft_strncmp(shell->env[i], name, len) == 0 
            && shell->env[i][len] == '=')
        {
            break;
        }
        i++;
    }
    
    if (!shell->env[i])
        return (0);
    
    count = 0;
    while (shell->env[count])
        count++;
    
    new_env = malloc(sizeof(char *) * count);
    if (!new_env)
        return (1);
    
    j = 0;
    int k = 0;
    while (shell->env[j])
    {
        if (j != i)
        {
            new_env[k] = shell->env[j];
            k++;
        }
        else
        {
            free(shell->env[j]);
        }
        j++;
    }
    new_env[k] = NULL;
    
    free(shell->env);
    shell->env = new_env;
    
    return (0);
}