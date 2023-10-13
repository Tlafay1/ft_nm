/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_sorted.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tlafay <tlafay@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/01 09:43:47 by tlafay            #+#    #+#             */
/*   Updated: 2023/10/13 17:19:23 by tlafay           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void ft_lstadd_sorted(t_list **head, t_list *new, int (*compare)(t_list *, t_list *))
{
    t_list *current = *head;
    t_list *previous = NULL;

    while (current && (*compare)(current, new) < 0) {
        previous = current;
        current = current->next;
    }

    // Handle the case where elements have the same name
    while (current && (*compare)(current, new) == 0) {
        previous = current;
        current = current->next;
    }

    if (!previous) {
        new->next = *head;
        *head = new;
    } else {
        new->next = previous->next;
        previous->next = new;
    }
}

