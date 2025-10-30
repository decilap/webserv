/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddecilap <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 16:17:28 by ddecilap          #+#    #+#             */
/*   Updated: 2024/11/22 17:25:55 by ddecilap         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	unsigned char		*dstp;
	unsigned const char	*srcp;

	if (dst == NULL && src == NULL)
		return (NULL);
	dstp = (unsigned char *)dst;
	srcp = (unsigned char *)src;
	while (n--)
	{
		*dstp++ = *srcp++;
	}
	return (dst);
}
