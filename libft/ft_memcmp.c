/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtawil <mtawil@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 16:01:47 by mtawil            #+#    #+#             */
/*   Updated: 2024/11/17 11:19:47 by mtawil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	const unsigned char	*pointer1;
	const unsigned char	*pointer2;

	pointer1 = s1;
	pointer2 = s2;
	while (n--)
	{
		if (*pointer1 != *pointer2)
		{
			return (*pointer1 - *pointer2);
		}
		pointer1++;
		pointer2++;
	}
	return (0);
}
