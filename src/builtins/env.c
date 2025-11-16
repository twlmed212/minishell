/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtawil <mtawil@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 02:45:56 by mtawil            #+#    #+#             */
/*   Updated: 2025/11/17 00:09:13 by mtawil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int builtin_env(char **args, t_shell *shell)
{
    int i;
    
    (void)args;
    
    i = 0;
    while (shell->env[i])
    {
        printf("%s\n", shell->env[i]);
        i++;
    }
    
    return (0);
}