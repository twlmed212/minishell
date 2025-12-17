/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtawil <mtawil@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 18:10:24 by mtawil            #+#    #+#             */
/*   Updated: 2025/12/17 16:45:46 by mtawil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*result;
	size_t	s1_len;
	size_t	s2_len;
	size_t	i;

	s1_len = ft_strlen(s1);
	s2_len = ft_strlen(s2);
	i = 0;
	result = ft_malloc((s1_len + s2_len + 1) * sizeof(char));
	if (result == NULL)
		return (NULL);
	while (i < (s1_len + s2_len))
	{
		if (i < s1_len)
			result[i] = s1[i];
		if (i < s2_len)
			result[s1_len + i] = s2[i];
		i++;
	}
	result[i] = '\0';
	return (result);
}
