/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddecilap <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 13:37:05 by ddecilap          #+#    #+#             */
/*   Updated: 2024/11/22 14:23:14 by ddecilap         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	unsigned char		*dstp;
	unsigned const char	*srcp;

	if (!src && !dst)
		return (NULL);
	dstp = (unsigned char *)dst;
	srcp = (unsigned char *)src;
	if (src < dst)
	{
		while (len != 0)
		{
			len--;
			dstp[len] = srcp[len];
		}
	}
	else
	{
		ft_memcpy(dst, src, len);
	}
	return (dst);
}
