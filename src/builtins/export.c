/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtawil <mtawil@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 02:46:01 by mtawil            #+#    #+#             */
/*   Updated: 2025/12/01 03:53:21 by mtawil           ###   ########.fr       */
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

static int print_env_sorted(t_env_and_exit *shell)
{
    int i;
    int j;
    char *temp;

    i = 0;
    while (shell->env[i])
    {
        j = i + 1;
        while (shell->env[j])
        {
            if (ft_strcmp(shell->env[i], shell->env[j]) > 0)
            {
                temp = shell->env[i];
                shell->env[i] = shell->env[j];
                shell->env[j] = temp;
            }
            j++;
        }
        i++;
    }
    i = 0;
    while (shell->env[i])
        printf("declare -x %s\n", shell->env[i++]);
    return (0);
}

static int print_error(char *arg, char *msg)
{
    ft_perror("minishell: export: `");
    ft_perror(arg);
    ft_perror(msg);
    return (1);
}

int builtin_export(char **args, t_env_and_exit *shell)
{
    char *equal_sign;
    char *name;
    char *value;
    char *clean_value;
    int ret;
    
    ret = 0;
    if (!args[1])
        return (print_env_sorted(shell));
    
    int i = 1;
    while (args[i])
    {

        if (args[i][0] == '=')
        {
            ret = print_error(args[i++], "': not a valid identifier\n");
            continue;
        }
        
        equal_sign = ft_strchr(args[i], '=');
        if (!equal_sign)
        {
            if (!is_name_valid(args[i]))
            {
                ret = print_error(args[i++], "': not a valid identifier\n");
                continue;   
            }
            shell->err = 10;
            set_env_value(args[i], "", shell);
            shell->err = 0;
            i++;
            continue;
        }
        
        name = ft_substr(args[i], 0, equal_sign - args[i]);
        if (!name)
        {
            ret = 1;
            i++;
            continue;
        }
        
        if (!is_name_valid(name))
        {
            ret = print_error(args[i++], "': not a valid identifier\n");
            free(name);
            continue;
        }
        
        value = ft_strdup(equal_sign + 1);
        if (!value)
        {
            free(name);
            ret = 1;
            i++;
            continue;
        }
        
        clean_value = remove_quotes(value);
        free(value);
        printf("Setting export: %s=%s\n", name, clean_value);
        if (!clean_value)
        {
            free(name);
            ret = 1;
            i++;
            continue;
        }
        printf("Calling set_env_value for %s\n", name);
        printf("Value to set: %s\n", clean_value);
        if (set_env_value(name, clean_value, shell) != 0)
            ret = 1;
        
        free(name);
        free(clean_value);
        i++;
    }
    
    return (ret);
}
