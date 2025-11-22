/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtawil <mtawil@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 02:46:18 by mtawil            #+#    #+#             */
/*   Updated: 2025/11/22 06:11:24 by mtawil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int in_quotes(char *str, int pos)
{
    int i;
    char quote;
    
    i = 0;
    quote = 0;
    
    while (i < pos)
    {
        if ((str[i] == '\'' || str[i] == '"') && !quote)
            quote = str[i];
        else if (str[i] == quote)
            quote = 0;
        i++;
    }
    
    return (quote != 0);
}

static int count_tokens(char *str)
{
    int i;
    int count;
    
    i = 0;
    count = 0;
    
    while (str[i])
    {
        while (str[i] == ' ')
            i++;
        
        if (str[i])
        {
            count++;
            while (str[i] && (str[i] != ' ' || in_quotes(str, i)))
                i++;
        }
    }
    
    return (count);
}

static char *extract_token(char *str, int *pos)
{
    int start;
    int len;
    
    while (str[*pos] == ' ')
        (*pos)++;
    
    start = *pos;
    len = 0;
    
    while (str[*pos] && (str[*pos] != ' ' || in_quotes(str, *pos)))
    {
        len++;
        (*pos)++;
    }
    
    return (ft_substr(str, start, len));
}

char **parse_command(char *input)
{
    char    **tokens;
    int     count;
    int     i;
    int     pos;
    
    if (!input)
        return (NULL);
    
    count = count_tokens(input);
    
    tokens = malloc(sizeof(char *) * (count + 1));
    if (!tokens)
        return (NULL);
    
    pos = 0;
    i = 0;
    while (i < count)
    {
        tokens[i] = extract_token(input, &pos);
        if (!tokens[i])
        {
            free_array(tokens);
            return (NULL);
        }
        i++;
    }
    tokens[i] = NULL;
    
    return (tokens);
}

void free_array(char **arr)
{
    int i;
    
    if (!arr)
        return;
    
    i = 0;
    while (arr[i])
    {
        free(arr[i]);
        i++;
    }
    free(arr);
}