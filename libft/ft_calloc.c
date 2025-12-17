/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtawil <mtawil@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 10:33:09 by mtawil            #+#    #+#             */
/*   Updated: 2025/12/17 16:45:46 by mtawil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	void	*x;
	size_t	i;

	i = 0;
	x = (char *)ft_malloc(nmemb * size);
	if (x == NULL)
		return (NULL);
	while (i < nmemb * size)
	{
		((char *)x)[i] = 0;
		i++;
	}
	return (x);
}
