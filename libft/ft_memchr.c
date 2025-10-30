/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddecilap <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 14:20:06 by ddecilap          #+#    #+#             */
/*   Updated: 2024/11/18 14:20:09 by ddecilap         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	const unsigned char	*sp;

	sp = (unsigned char *)s;
	while (n--)
	{
		if (*sp == (unsigned char)c)
			return ((void *)sp);
		sp++;
	}
	return (NULL);
}
