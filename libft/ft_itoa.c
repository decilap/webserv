/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddecilap <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 19:12:14 by ddecilap          #+#    #+#             */
/*   Updated: 2024/11/20 19:15:34 by ddecilap         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*ft_print(char *result, int number, int len)
{
	result[len] = '\0';
	while (number != 0)
	{
		result[--len] = (number % 10) + '0';
		number /= 10;
	}
	return (result);
}

static int	count_numbers(int c)
{
	int	count;

	count = 0;
	if (c <= 0)
		count = 1;
	while (c != 0)
	{
		c /= 10;
		count++;
	}
	return (count);
}

char	*ft_itoa(int c)
{
	size_t	len;
	char	*result;	
	int		number;

	len = count_numbers(c);
	if (c == -2147483648)
		return (ft_strdup("-2147483648"));
	result = (char *)malloc(sizeof(char) * (len + 1));
	if (!result)
		return (NULL);
	if (c < 0)
	{
		result[0] = '-';
		number = -c;
	}
	else
		number = c;
	if (c == 0)
	{
		result[0] = '0';
		result[1] = '\0';
		return (result);
	}
	ft_print(result, number, len);
	return (result);
}
