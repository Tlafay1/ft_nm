/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_sorted.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlafay <tlafay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/01 09:43:47 by tlafay            #+#    #+#             */
/*   Updated: 2023/03/01 09:44:53 by tlafay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_sorted(t_list **head, t_list *new,
	int (*compare)(t_list *, t_list *))
{
	t_list	*current;

	if (!*head || (*compare)(*head, new) >= 0)
	{
		new->next = *head;
		*head = new;
	}
	else
	{
		current = *head;
		while (current->next && (*compare)(current->next, new) < 0)
			current = current->next;
		new->next = current->next;
		current->next = new;
	}
}
