/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtawil <mtawil@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 02:46:01 by mtawil            #+#    #+#             */
/*   Updated: 2025/11/28 20:43:31 by mtawil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int is_var_valid(char c, int is_first)
{
    if (is_first)
        return (ft_isalpha(c) || c == '_');
    return (ft_isalnum(c) || c == '_');
}

static int is_name_valid(char *name)
{
    int i;

    if (!name || !name[0])
        return (0);
    
    if (!is_var_valid(name[0], 1))
        return (0);
    
    i = 1;
    while (name[i])
    {
        if (!is_var_valid(name[i], 0))
            return (0);
        i++;
    }
    return (1);
}
int builtin_export(char **args, t_env_and_exit *shell)
{
    char *equal_sign;
    char *name;
    char *value;
    char *clean_value;
    int i;
    
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
    int ret;
    while (args[i])
    {
        if (args[i][0] == '=')
        {
            ft_perror("minishell: export: `");
            ft_perror(args[i]);
            ft_perror("': not a valid identifier\n");
            ret = 1;
            i++;
            continue;
        }
        
        equal_sign = ft_strchr(args[i], '=');
        if (!equal_sign)
        {
            if (!is_name_valid(args[i]))
            {
                ft_perror("minishell: export: `");
                ft_perror(args[i]);
                ft_perror("': not a valid identifier\n");
                ret = 1;
            }
            i++;
            continue;
        }
        
        // Extract name (everything before '=')
        name = ft_substr(args[i], 0, equal_sign - args[i]);
        if (!name)
        {
            ret = 1;
            i++;
            continue;
        }
        
        // Validate the identifier
        if (!is_name_valid(name))
        {
            ft_perror("minishell: export: `");
            ft_perror(args[i]);
            ft_perror("': not a valid identifier\n");
            free(name);
            ret = 1;
            i++;
            continue;
        }
        
        // Extract value (everything after '=')
        value = ft_strdup(equal_sign + 1);
        if (!value)
        {
            free(name);
            ret = 1;
            i++;
            continue;
        }
        
        // Remove quotes from value if present
        clean_value = remove_quotes(value);
        free(value);
        
        if (!clean_value)
        {
            free(name);
            ret = 1;
            i++;
            continue;
        }
        
        // Set the environment variable
        if (set_env_value(name, clean_value, shell) != 0)
            ret = 1;
        
        free(name);
        free(clean_value);
        i++;
    }
    
    return (ret);
}
