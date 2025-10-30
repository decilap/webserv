/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddecilap <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 13:37:57 by ddecilap          #+#    #+#             */
/*   Updated: 2024/11/18 13:38:11 by ddecilap         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s1)
{
	char	*s1p;
	size_t	len;

	len = ft_strlen(s1);
	s1p = (char *)malloc(sizeof(char) * (len + 1));
	if (!s1p)
		return (NULL);
	ft_memcpy(s1p, s1, len);
	s1p[len] = '\0';
	return (s1p);
}
