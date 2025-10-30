/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddecilap <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 13:38:46 by ddecilap          #+#    #+#             */
/*   Updated: 2024/11/27 17:36:01 by ddecilap         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	ft_crash_func(void)
{
	int	*ptr;

	ptr = NULL;
	*ptr = 42;
}

size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize)
{
	size_t	src_len;
	size_t	copy_len;

	if (!dst || !src)
	{
		ft_crash_func();
		return (0);
	}
	src_len = ft_strlen(src);
	if (dstsize > 0)
	{
		if (src_len < dstsize)
			copy_len = src_len;
		else
			copy_len = dstsize - 1;
		ft_memcpy(dst, src, copy_len);
		dst[copy_len] = '\0';
	}
	return (src_len);
}
