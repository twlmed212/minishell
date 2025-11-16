/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtawil <mtawil@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 02:45:52 by mtawil            #+#    #+#             */
/*   Updated: 2025/11/16 05:30:13 by mtawil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int builtin_cd(char **args)
{
    char *path;
    
    if (!args[1])
    {
        path = getenv("HOME");
        if (!path)
        {
            printf("cd: HOME not set\n");
            return (1);
        }
    }
    else
    {
        path = args[1];
    }
    
    if (chdir(path) == -1)
    {
        perror("cd");
        return (1);
    }
    
    return (0);
}