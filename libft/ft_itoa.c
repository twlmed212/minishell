/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mtawil <mtawil@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 11:49:15 by mtawil            #+#    #+#             */
/*   Updated: 2024/11/18 17:22:58 by mtawil           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	string_size(int n)
{
	int	size;

	size = 0;
	if (n == 0)
		return (++size);
	if (n < 0)
	{
		if (n == -2147483648)
		{
			n++;
		}
		n *= -1;
		size++;
	}
	while (n > 0)
	{
		n /= 10;
		size++;
	}
	return (size);
}

static int	signl_handler(char *result, int *int_max, int n)
{
	if (n < 0)
	{
		result[0] = '-';
		if (n == -2147483648)
		{
			n++;
			*int_max = 1;
		}
		n *= -1;
	}
	return (n);
}

char	*ft_itoa(int n)
{
	char	*result;
	int		nmbr_length;
	int		int_max;

	int_max = 0;
	nmbr_length = string_size(n);
	result = malloc((nmbr_length + 1) * sizeof(char));
	if (!result)
		return (NULL);
	if (n == 0)
		result[0] = '0';
	n = signl_handler(result, &int_max, n);
	result[nmbr_length--] = 0;
	while (n > 0)
	{
		if (int_max && nmbr_length == 10)
			result[nmbr_length] = '8';
		else
			result[nmbr_length] = (n % 10) + 48;
		n /= 10;
		nmbr_length--;
	}
	return (result);
}
