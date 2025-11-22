/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtawil <mtawil@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 11:35:25 by mtawil            #+#    #+#             */
/*   Updated: 2024/11/18 13:50:47 by mtawil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	i;
	size_t	srclen;
	size_t	destlen;
	size_t	y;

	i = 0;
	destlen = ft_strlen(dst);
	y = destlen;
	srclen = ft_strlen(src);
	if (size == 0)
		return (srclen);
	if (destlen >= size)
		return (size + srclen);
	while (src[i] && y < size - 1)
	{
		dst[y] = src[i];
		y++;
		i++;
	}
	dst[y] = '\0';
	return (srclen + destlen);
}
