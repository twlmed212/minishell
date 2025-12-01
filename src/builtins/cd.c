/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtawil <mtawil@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 02:45:52 by mtawil            #+#    #+#             */
/*   Updated: 2025/12/01 03:18:15 by mtawil           ###   ########.fr       */
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
            return (ft_perror("cd: HOME not set\n") , 1);
    }
    else if (args[2])
    {
        printf("cd: too many arguments\n");
        return 1;
    }
    else
        path = args[1];

    if (chdir(path) != 0)
    {
        ft_perror("minishell: ");
        ft_perror(path);
        ft_perror(" : No such file or directory\n");
        return 1;
    }
    return 0;
}
