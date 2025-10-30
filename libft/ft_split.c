/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddecilap <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 19:33:57 by ddecilap          #+#    #+#             */
/*   Updated: 2024/11/27 17:31:44 by ddecilap         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	ft_free_array(char **result)
{
	int	i;

	i = 0;
	if (!result)
		return ;
	while (result[i])
	{
		free(result[i]);
		i++;
	}
	free(result);
}

static int	ft_count_word(const char *str, char c)
{
	int	count;
	int	in_word;

	in_word = 0;
	count = 0;
	while (*str)
	{
		if (*str == c)
			in_word = 0;
		else if (!in_word)
		{
			in_word = 1;
			count++;
		}
		str++;
	}
	return (count);
}

static char	*ft_copy_word(const char *start, size_t len)
{
	char	*word;

	word = malloc(len + 1);
	if (!word)
		return (NULL);
	ft_memcpy(word, start, len);
	word[len] = '\0';
	return (word);
}

static int	ft_split_string(const char *str, char c, char **result)
{
	int			i;
	const char	*start;

	i = 0;
	while (*str)
	{
		if (*str != c)
		{
			start = str;
			while (*str && *str != c)
				str++;
			result[i] = ft_copy_word(start, str - start);
			if (!result[i])
				return (0);
			i++;
		}
		else
			str++;
	}
	return (1);
}

char	**ft_split(const char *str, char c)
{
	char	**result;
	int		word_count;

	if (!str)
		return (NULL);
	word_count = ft_count_word(str, c);
	result = malloc((word_count + 1) * sizeof(char *));
	if (!result)
		return (NULL);
	if (!ft_split_string(str, c, result))
	{
		ft_free_array(result);
		return (NULL);
	}
	result[word_count] = (NULL);
	return (result);
}
