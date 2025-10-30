/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bonus.lstsize.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddecilap <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 17:36:47 by ddecilap          #+#    #+#             */
/*   Updated: 2024/11/26 12:44:40 by ddecilap         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_lstsize(t_list *lst)
{
	size_t	lst_size;
	t_list	*current;

	current = lst;
	lst_size = 0;
	while (current != NULL)
	{
		current = current->next;
		lst_size++;
	}
	return (lst_size);
}
