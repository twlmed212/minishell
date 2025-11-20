/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_args.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtawil <mtawil@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/20 02:38:26 by mtawil            #+#    #+#             */
/*   Updated: 2025/11/20 02:38:27 by mtawil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char **expand_args(char **args, t_shell *shell)
{
    char **expanded;
    int i;
    int count;
    
    if (!args)
        return (NULL);
    
    count = 0;
    while (args[count])
        count++;
    
    expanded = malloc(sizeof(char *) * (count + 1));
    if (!expanded)
        return (NULL);
    
    i = 0;
    while (args[i])
    {
        expanded[i] = expand_string(args[i], shell);
        i++;
    }
    expanded[i] = NULL;
    
    return (expanded);
}