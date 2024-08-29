/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabdou <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/01 20:40:08 by aabdou            #+#    #+#             */
/*   Updated: 2021/12/03 14:50:27 by aabdou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"libft.h"

void	*ft_calloc(size_t count, size_t size)
{	
	void	*i;

	i = (void *)malloc(count * size);
	if (!i)
		return (NULL);
	ft_bzero(i, count * size);
	return (i);
}
