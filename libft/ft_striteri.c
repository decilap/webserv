/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_striteri.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddecilap <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 19:31:02 by ddecilap          #+#    #+#             */
/*   Updated: 2024/11/20 19:32:13 by ddecilap         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
void test_func(unsigned int i, char *s)
{
    if (i % 2 == 0 && (*s >= 'A' && *s <= 'Z'))
		*s -= 32;  
    else if (i % 2 != 0 && (*s >= 'a' && *s <= 'z'))
		*s += 32; 
}
*/
void	ft_striteri(char *s, void (*f)(unsigned int, char *))
{
	unsigned int	i;

	if (!s || !f)
		return ;
	i = 0;
	while (s[i])
	{
		f(i, &s[i]);
		i++;
	}
}
