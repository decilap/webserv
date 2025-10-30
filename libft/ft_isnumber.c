/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isnumber.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ddecilap <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 15:19:09 by ddecilap          #+#    #+#             */
/*   Updated: 2024/11/09 16:16:56 by ddecilap         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>

int ft_isnumber(char *str)
{
    int has_digit;

    if (str == NULL || *str == '\0')
        return (0);
    has_digit = 0;
    if (*str == '+' || *str == '-')
        str++;
    while (*str) 
    {
        if (ft_isdigit(*str))
            has_digit = 1;
        else
            return (0);
        str++;
    }
    return has_digit;
}
