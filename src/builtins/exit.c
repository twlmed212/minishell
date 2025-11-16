/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtawil <mtawil@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 02:45:59 by mtawil            #+#    #+#             */
/*   Updated: 2025/11/16 02:46:00 by mtawil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int builtin_exit(char **args)
{
     int exit_code;
    
    printf("exit\n");
    
    if (!args[1])
        exit(0);
    
    exit_code = ft_atoi(args[1]);
    free_array(args);
    exit(exit_code);
    
    return (0);
}