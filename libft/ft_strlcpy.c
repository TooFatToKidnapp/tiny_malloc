/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabdou <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/01 20:52:06 by aabdou            #+#    #+#             */
/*   Updated: 2021/12/01 20:52:09 by aabdou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"libft.h"

size_t	ft_strlcpy(char *dst, const char *src, size_t ds)
{
	size_t	i;

	i = 0;
	if (ds > 0)
	{	
		while (src[i] && i < (ds - 1))
		{	
			dst[i] = src[i];
			i++;
		}	
		dst[i] = 0;
	}
	while (src[i])
		i++;
	return (i);
}
