/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddecilap <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 13:39:31 by ddecilap          #+#    #+#             */
/*   Updated: 2024/11/27 17:36:39 by ddecilap         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	ft_crash_func(void)
{
	int	*ptr;

	ptr = NULL;
	*ptr = 42;
}

char	*ft_strrchr(const char *s, int c)
{
	char	*end;
	int		s_len;

	if (!s)
		ft_crash_func();
	s_len = ft_strlen(s);
	end = (char *)s + s_len;
	if (c == '\0')
		return ((char *)end);
	while (end >= s)
	{
		if (*end == (char)c)
			return (end);
		end--;
	}
	return (NULL);
}
