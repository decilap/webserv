/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddecilap <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 14:36:23 by ddecilap          #+#    #+#             */
/*   Updated: 2024/11/27 17:45:26 by ddecilap         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strtrim(char const *s1, char const *set)
{
	const char	*start;
	const char	*end;
	size_t		trim_len;
	char		*str_trim;

	if (!s1 || !set)
		return (NULL);
	start = s1;
	while (*start && ft_strchr(set, *start))
		start++;
	end = s1 + ft_strlen(s1);
	while (end > start && ft_strchr(set, *(end - 1)))
		end--;
	trim_len = end - start;
	str_trim = (char *)malloc(trim_len + 1);
	if (!str_trim)
		return (NULL);
	ft_memcpy(str_trim, start, trim_len);
	str_trim[trim_len] = '\0';
	return (str_trim);
}
