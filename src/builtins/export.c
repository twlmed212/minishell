/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtawil <mtawil@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 02:46:01 by mtawil            #+#    #+#             */
/*   Updated: 2025/12/01 14:27:51 by mtawil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int is_var_valid(char c, int is_first)
{
    if (is_first)
        return (ft_isalpha(c) || c == '_');
    return (ft_isalnum(c) || c == '_');
}

int builtin_export(char **args, t_env_and_exit *shell)
{
    char *clean_value;
    int ret;
    char *name;
    int i;

    i = 1;
    ret = 0;
    if (!args[1])
        return (print_env_sorted(shell));
    while (args[i])
    {
        name = get_name(args[i], shell, &ret, &i);
        if (!name)
            continue;
       
        clean_value = get_value(args[i], name, &ret, &i);
        if (!clean_value)
            continue;
        if (set_env_value(name, clean_value, shell) != 0)
            ret = 1;
        free(name);
        free(clean_value);
        i++;
    }
    return (ret);
}
