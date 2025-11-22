/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncmp.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtawil <mtawil@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 15:18:35 by mtawil            #+#    #+#             */
/*   Updated: 2024/11/18 17:11:58 by mtawil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	unsigned char	*se1;
	unsigned char	*se2;
	size_t			i;

	se1 = (unsigned char *)s1;
	se2 = (unsigned char *)s2;
	i = 0;
	if (n == 0)
		return (0);
	while (i < n && (se1[i] || se2[i]))
	{
		if (se1[i] != se2[i])
			return (se1[i] - se2[i]);
		i++;
	}
	return (0);
}
