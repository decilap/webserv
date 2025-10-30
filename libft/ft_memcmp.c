/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddecilap <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 13:21:22 by ddecilap          #+#    #+#             */
/*   Updated: 2024/11/18 13:21:25 by ddecilap         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	unsigned char		*ps1;
	const unsigned char	*ps2;

	ps1 = (unsigned char *)s1;
	ps2 = (unsigned const char *)s2;
	while (n--)
	{
		if (*ps1 != *ps2)
			return ((int)(*ps1 - *ps2));
		ps1++;
		ps2++;
	}
	return (0);
}
