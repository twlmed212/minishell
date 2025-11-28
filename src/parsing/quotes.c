/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtawil <mtawil@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 02:46:22 by mtawil            #+#    #+#             */
/*   Updated: 2025/11/27 15:09:05 by mtawil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	is_quote(char c)
{
	return (c == '\'' || c == '"');
}

char *remove_quotes(char *str)
{
    char    *result;
    int     i;
    int     j;
    char    quote;
    
    if (!str)
        return (ft_strdup(""));
    
    result = malloc(ft_strlen(str) + 1);
    if (!result)
        return (NULL);
    
    i = 0;
    j = 0;
    quote = 0;
    
    while (str[i])
    {
        if (is_quote(str[i]) && !quote)
        {
            quote = str[i];
            i++;
        }
        else if (str[i] == quote)
        {
            quote = 0;
            i++;
        }
        else
        {
            result[j] = str[i];
            j++;
            i++;
        }
    }
    
    result[j] = '\0';
    return (result);
}

int check_unclosed_quotes(const char *line){

    int i = 0;
    char c;
    while (line[i])
    {
        if (line[i] == '\'' || line[i] == '\"')
        {
            c = line[i];
            i++;
            while (line[i] != c && line[i])
                i++;
            if (!line[i])
                return (-1);
            i++;
        }
        else
            i++;
    }
    return (1);
}