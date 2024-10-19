/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aabdou <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/15 15:37:12 by aabdou            #+#    #+#             */
/*   Updated: 2021/11/24 13:44:54 by aabdou           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

t_list *ft_lstnew(void *chunk)
{
	t_list *head;

	head = (t_list *)malloc(sizeof(*head));
	if (!head)
		return (NULL);
	head->content = chunk;
	head->next = NULL;
	return (head);
}
