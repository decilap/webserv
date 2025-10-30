/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddecilap <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 13:39:09 by ddecilap          #+#    #+#             */
/*   Updated: 2024/11/27 17:35:49 by ddecilap         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	ft_crash_func(void)
{
	int	*ptr;

	ptr = NULL;
	*ptr = 42;
}

char	*ft_strnstr(const char *haystack, const char *needle, size_t len)
{
	size_t		i;
	size_t		needle_len;

	if (!haystack || !needle)
		ft_crash_func();
	if (!*needle)
		return ((char *)haystack);
	needle_len = ft_strlen(needle);
	if (len < needle_len)
		return (NULL);
	i = 0;
	while (i <= len - needle_len && haystack[i])
	{
		if (ft_strncmp(&haystack[i], needle, needle_len) == 0)
			return ((char *)&haystack[i]);
		i++;
	}
	return (NULL);
}
