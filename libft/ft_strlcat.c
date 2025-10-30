/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddecilap <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 13:38:27 by ddecilap          #+#    #+#             */
/*   Updated: 2024/11/27 17:34:36 by ddecilap         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	ft_crash_func(void)
{
	int	*ptr;

	ptr = NULL;
	*ptr = 42;
}

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t	src_len;
	size_t	dst_len;
	size_t	copy_len;
	char	*pdst;

	if (!dst || !src)
		ft_crash_func();
	src_len = ft_strlen(src);
	pdst = dst;
	while (*pdst && (size_t)(pdst - dst) < dstsize)
		pdst++;
	dst_len = pdst - dst;
	if (dst_len >= dstsize)
		return (dstsize + src_len);
	copy_len = dstsize - dst_len - 1;
	if (copy_len > src_len)
		copy_len = src_len;
	ft_memcpy(pdst, src, copy_len);
	pdst[copy_len] = '\0';
	return (dst_len + src_len);
}
