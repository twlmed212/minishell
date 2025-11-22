/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtawil <mtawil@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 18:54:23 by mtawil            #+#    #+#             */
/*   Updated: 2024/11/18 14:16:20 by mtawil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strtrim(char const *s1, char const *set)
{
	size_t	len_s;
	size_t	i;

	i = 0;
	if (!*s1)
		return (ft_strdup(""));
	while (ft_strchr(set, s1[i]) && s1[i])
	{
		i++;
	}
	len_s = ft_strlen(s1);
	while (ft_strchr(set, s1[len_s - 1]) && len_s > i)
	{
		len_s--;
	}
	return (ft_substr(s1, i, len_s - i));
}
