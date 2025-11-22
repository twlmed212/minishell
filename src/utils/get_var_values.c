/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_var_values.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtawil <mtawil@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/20 02:38:32 by mtawil            #+#    #+#             */
/*   Updated: 2025/11/22 04:48:07 by mtawil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int is_var_char(char c)
{
    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
        return (1);
    if (c >= '0' && c <= '9')
        return (1);
    if (c == '_')
        return (1);
    return (0);
}
char *extract_var_name(char *str)
{
    int i;
    int len;
    
    if (str[0] != '$')
        return (NULL);
    
    if (str[1] == '\0')
        return (NULL);
    
    if (str[1] == '?')
        return (ft_strdup("?"));
    
    i = 1;
    len = 0;
    while (str[i] && is_var_char(str[i]))
    {
        len++;
        i++;
    }
    
    if (len == 0)
        return (NULL);
    
    return (ft_substr(str, 1, len));
}