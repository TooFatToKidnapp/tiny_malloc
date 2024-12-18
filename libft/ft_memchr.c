/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabdou <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/01 20:44:37 by aabdou            #+#    #+#             */
/*   Updated: 2021/12/03 14:50:46 by aabdou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	size_t	i;
	const unsigned char * c_s = s;
	i = 0;
	while (i < n)
	{
		if (c_s[i] == (unsigned char) c)
			return ((void *)&c_s[i]);
		i++;
	}
	return (NULL);
}
