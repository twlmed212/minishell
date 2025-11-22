/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtawil <mtawil@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 17:55:25 by mtawil            #+#    #+#             */
/*   Updated: 2024/11/18 17:17:03 by mtawil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	i;
	size_t	j;
	size_t	ltf;

	i = 0;
	ltf = ft_strlen(little);
	if (!big && !little)
		return (0);
	if (*little == '\0')
		return ((char *)big);
	while (big[i] && i < len)
	{
		j = 0;
		while ((little[j]) && (i + j) < len && (big[i + j] == little[j]))
			j++;
		if (ltf == j)
			return ((char *)(big + i));
		i++;
	}
	return (NULL);
}
