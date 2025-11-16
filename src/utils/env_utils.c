/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtawil <mtawil@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 02:46:28 by mtawil            #+#    #+#             */
/*   Updated: 2025/11/17 00:10:35 by mtawil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char **copy_env(char **envp)
{
    char    **new_env;
    int     i;
    int     count;
    
    count = 0;
    while (envp[count])
        count++;
    
    new_env = malloc(sizeof(char *) * (count + 1));
    if (!new_env)
        return (NULL);
    
    i = 0;
    while (envp[i])
    {
        new_env[i] = ft_strdup(envp[i]);
        if (!new_env[i])
        {
            free_array(new_env);
            return (NULL);
        }
        i++;
    }
    new_env[i] = NULL;
    
    return (new_env);
}

char *get_env_value(char *name, t_shell *shell)
{
    int     i;
    int     len;
    
    if (!name || !shell->env)
        return (NULL);
    
    len = ft_strlen(name);
    i = 0;
    while (shell->env[i])
    {
        if (ft_strncmp(shell->env[i], name, len) == 0 
            && shell->env[i][len] == '=')
            return (&shell->env[i][len + 1]);
        i++;
    }
    
    return (NULL);
}

int set_env_value(char *name, char *value, t_shell *shell)
{
    char    *new_var;
    char    *temp;
    int     i;
    int     len;
    
    temp = ft_strjoin(name, "=");
    if (!temp)
        return (1);
    new_var = ft_strjoin(temp, value);
    free(temp);
    if (!new_var)
        return (1);
    
    len = ft_strlen(name);
    i = 0;
    while (shell->env[i])
    {
        if (ft_strncmp(shell->env[i], name, len) == 0 
            && shell->env[i][len] == '=')
        {
            free(shell->env[i]);
            shell->env[i] = new_var;
            return (0);
        }
        i++;
    }
    
    char **new_env;
    int count = 0;
    while (shell->env[count])
        count++;
    
    new_env = malloc(sizeof(char *) * (count + 2));
    if (!new_env)
    {
        free(new_var);
        return (1);
    }
    
    i = 0;
    while (i < count)
    {
        new_env[i] = shell->env[i];
        i++;
    }
    
    new_env[i] = new_var;
    new_env[i + 1] = NULL;
    
    free(shell->env);
    shell->env = new_env;
    
    return (0);
}
