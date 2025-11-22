/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtawil <mtawil@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 02:46:22 by mtawil            #+#    #+#             */
/*   Updated: 2025/11/22 06:10:54 by mtawil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

#include "../../include/minishell.h"

int is_quote(char c)
{
    return (c == '\'' || c == '"');
}

char *remove_quotes(char *str)
{
    int     len;
    char    outer_quote;
    
    if (!str || !str[0])
        return (ft_strdup(""));
    
    len = ft_strlen(str);
    
    if (len >= 2 && is_quote(str[0]) && str[0] == str[len - 1])
    {
        outer_quote = str[0];
        
        int i = 1;
        int quote = 0;
        while (i < len - 1)
        {
            if (is_quote(str[i]) && str[i] != outer_quote && !quote)
                quote = str[i];
            else if (str[i] == quote)
                quote = 0;
            i++;
        }
        
        if (!quote)
            return (ft_substr(str, 1, len - 2));
    }
    
    return (ft_strdup(str));
}

int should_expand(char *str, int pos)
{
    int     i;
    char    quote;
    
    i = 0;
    quote = 0;
    
    while (i < pos)
    {
        if (is_quote(str[i]) && !quote)
            quote = str[i];
        else if (str[i] == quote)
            quote = 0;
        i++;
    }
    
    if (quote == '\'')
        return (0);
    
    return (1);
}