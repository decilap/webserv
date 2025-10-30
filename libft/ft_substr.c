/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddecilap <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 19:04:54 by ddecilap          #+#    #+#             */
/*   Updated: 2024/11/20 19:09:00 by ddecilap         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t size)
{
	const char	*pstart;
	char		*pstr;
	size_t		len;

	if (!s)
		return (NULL);
	len = ft_strlen(s);
	if (start >= len)
		return (ft_calloc(1, sizeof(char)));
	if (size > len - start)
		size = len - start;
	pstr = (char *)malloc(size + 1);
	if (!pstr)
		return (NULL);
	pstart = s + start;
	ft_memcpy(pstr, pstart, size);
	pstr[size] = '\0';
	return (pstr);
}
